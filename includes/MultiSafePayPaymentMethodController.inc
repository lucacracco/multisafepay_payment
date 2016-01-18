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
    'creator_order' => '',
    'gateway' => NULL,
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
  // TODO: implement in this form the configuration form, validate function and submit from MultiSafePayPaymentCreatorOrderInterface.
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
        $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_FAILED));
      }
    }
    else {
      $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_FAILED));
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
    $creator_order_id = $payment->method->controller_data['creator_order'];

    $definition = multisafepay_payment_creator_order_info($creator_order_id);
    if (!$definition) {
      return NULL;
    }

    try {
      $creator_order = call_user_func(array(
        $definition['class'],
        'create'
      ), $payment, $payment->method, $configuration = array(), $creator_order_id, $definition);
      $msp_order = $creator_order->createOrder();
    } catch (Exception $e) {
      return NULL;
    }

    return $msp_order;
  }

  /**
   * Start Transaction
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

        $message = t("Error: @error", array('@error' => $e->getMessage()));
        drupal_set_message("Error in communication with MultiSafepay Server. Contact Support.", 'error');
        watchdog('multisafepay_payment', $message, array(), WATCHDOG_ERROR);
        return NULL;
      }
    }
    else {
      // Library Not Loaded.
      $message = "Library Not Loaded. Contact Support.";
      drupal_set_message($message, 'error');
      watchdog('multisafepay_payment', $message, WATCHDOG_ERROR);
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

}
