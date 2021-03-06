<?php

/**
 * Contains MultiSafePayPaymentMethodController.
 */

/**
 * A base class for payment method controllers that talk to MultiSafePay.
 */
class MultiSafePayPaymentMethodController extends PaymentMethodController {

  /**
   * The production server.
   */
  const SERVER_PRODUCTION = 0;

  /**
   * The sandbox server.
   */
  const SERVER_SANDBOX = 1;

  /**
   * The production API server URL.
   */
  const URL_SERVER_PRODUCTION = 'https://api.multisafepay.com/v1/json/';

  /**
   * The sandbox API server URL.
   */
  const URL_SERVER_SANDBOX = 'https://testapi.multisafepay.com/v1/json/';

  /**
   * The toolkit version used.
   */
  const TOOLKIT_VERSION = '2.0.0';

  /**
   * {@inheritdoc}
   */
  public $controller_data_defaults = array(
    'api_server' => self::SERVER_SANDBOX,
    'api_key' => '',
    'plugin' => NULL,
    'configuration' => '',
  );

  /**
   * {@inheritdoc}
   */
  public $description = 'Provides an <a href="http://www.multisafepay.com/">MultiSafePay</a> payment method type.';

  /**
   * {@inheritdoc}
   */
  public $title = 'MultiSafePay';

  /**
   * {@inheritdoc}
   */
  public $payment_method_configuration_form_elements_callback = 'multisafepay_payment_payment_method_configuration_form_elements';

  /**
   * {@inheritdoc}
   */
  function validate(Payment $payment, PaymentMethod $payment_method, $strict) {
    parent::validate($payment, $payment_method, $strict);
  }

  /**
   * {@inheritdoc}
   */
  function execute(Payment $payment) {

    // Save payment
    entity_save('payment', $payment);

    // Construct MultiSafePay Order Base.
    $msp_order = $this->initOrder($payment);

    if (isset($msp_order)) {

      // Alter data MultiSafePay Order Base.
      drupal_alter('multisafepay_payment_msp_order', $msp_order, $payment);

      // Start Transaction
      $api_key = $payment->method->controller_data['api_key'];
      $api_server = $payment->method->controller_data['api_server'];
      $payment_link = $this->startTransaction($msp_order, $api_key, $api_server);

      // Check payment link.
      if (valid_url($payment_link) && url_is_external($payment_link)) {
        drupal_goto($payment_link);
      }
      else {
        // Set Failed status.
        $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_FAILED));

        // Print and log error.
        watchdog(
          'multisafepay_payment',
          t('Payment Link redirect not correct for payment \'@pid\' '),
          array('@pid' => $payment->pid),
          WATCHDOG_ERROR
        );
        drupal_set_message(t('Error! Contact Support'), 'error');
      }
    }
    else {
      // Print and log error.
      $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_FAILED));

      // Print and log error.
      watchdog(
        'multisafepay_payment',
        t('MSP Order not load for payment \'@pid\' '),
        array('@pid' => $payment->pid),
        WATCHDOG_ERROR
      );
      drupal_set_message(t('Error! Contact Support'), 'error');;
    }
  }

  /**
   * Load Order data.
   *
   * @param $payment
   * @return array|null
   */
  function initOrder($payment) {

    $msp_order = NULL;
    $plugin_id = $payment->method->controller_data['plugin'];

    $definition = multisafepay_payment_plugin_msp_order_info($plugin_id);
    if (!$definition) {
      return NULL;
    }

    // Create MSP Order.
    try {
      $configuration = unserialize($payment->method->controller_data['configuration']);
      $configuration = is_array($configuration) ? $configuration : array();
      $plugin = new $definition['class']($plugin_id, $definition, $configuration);
      $msp_order = $plugin->createOrder($payment);
    } catch (Exception $e) {

      // Log error and NULL return.
      watchdog(
        'multisafepay_payment',
        t('Plugin @plugin not found or error.'),
        array('@plugin' => $definition['class']),
        WATCHDOG_ERROR
      );
      return NULL;
    }

    return $msp_order;
  }

  /**
   * Start Transaction.
   * Communication with server MultiSafepay for retrieve the url redirect.
   */
  function startTransaction($msp_order, $api_key, $api_server) {

    // Load the library 'multisafepay_php' and check.
    if (($library = libraries_load('multisafepay_php')) && !empty($library['loaded'])) {

      // Use MultiSafePay Libraries.
      $msp = new Client();
      $msp->setApiKey($api_key);
      $msp->setApiUrl($this->serverURL($api_server));

      try {

        $msp_order = $msp->orders->post($msp_order);
        return $msp->orders->getPaymentLink();

      } catch (Exception $e) {
        drupal_set_message("Error in communication with MultiSafepay Server. Contact Support.", 'error');
        watchdog('multisafepay_payment', t("Error: @error"), array('@error' => $e->getMessage()), WATCHDOG_ERROR);
        return NULL;
      }
    }
    else {
      // Library Not Loaded.
      $message = "Library Not Loaded. Contact Support.";
      drupal_set_message($message, 'error');
      watchdog('multisafepay_payment', $message, array(), WATCHDOG_ERROR);
      return NULL;
    }
  }

  /**
   * Returns the API server URL.
   *
   * @throws InvalidArgumentException
   *
   * @param int $server
   *   One of the self::SERVER_* constants.
   *
   * @return string
   */
  function serverURL($server) {
    $urls = array(
      $this::SERVER_PRODUCTION => $this::URL_SERVER_PRODUCTION,
      $this::SERVER_SANDBOX => $this::URL_SERVER_SANDBOX,
    );
    if (array_key_exists($server, $urls)) {
      return url($urls[$server], array(
        'external' => TRUE,
      ));
    }
    else {
      throw new InvalidArgumentException(t('Server type does not exist.'));
    }
  }

  /**
   * Map Status from Payment and MultiSafepay.
   *
   * @param null $msp_status
   * @return array|string
   */
  function mapStatus($msp_status = NULL) {

    $msp_status = strtolower($msp_status);

    $map = array(
      'initialized' => PAYMENT_STATUS_NEW,
      // Rejected by the credit card company.
      'declined' => PAYMENT_STATUS_AUTHORIZATION_FAILED,
      // Canceled by the merchant (only applies to the status Initialised or Uncleared).
      'canceled' => PAYMENT_STATUS_FAILED,
      // Payment has been successfully completed.
      'completed' => PAYMENT_STATUS_SUCCESS,
      // Depending on the payment method unfinished transactions automatically expire after a predefined period.
      'expired' => PAYMENT_STATUS_EXPIRED,
      // Waiting for manual permission of the merchant to approve/disapprove the payment.
      'uncleared' => PAYMENT_STATUS_PENDING,
      // Payment has been refunded to the customer.
      'refunded' => PAYMENT_STATUS_CANCELLED,
      // The payment has been partially refunded to the customer.
      'partial_Refunded' => PAYMENT_STATUS_UNKNOWN,
      // Payout/refund has been temporary put on reserved, a temporary status, till the e-wallet has been checked on sufficient balance.
      'reserved' => PAYMENT_STATUS_PENDING,
      // Failed payment.
      'void' => PAYMENT_STATUS_FAILED,
      // Forced reversal of funds initiated by consumer’s issuing bank. Only applicable to direct debit and credit card payments.
      'chargedback' => PAYMENT_STATUS_UNKNOWN,
    );

    if (isset($msp_status)) {
      return isset($map[$msp_status]) ? $map[$msp_status] : PAYMENT_STATUS_UNKNOWN;
    }
    return $map;
  }

}
