<?php

/**
 * @file MultiSafePayPaymentCreatorOrderBase.inc
 * Contains MultiSafePayPaymentCreatorOrderBase.
 */

/**
 * Provides a base class for creator plugins.
 */
abstract class MultiSafePayPaymentCreatorOrderBase implements MultiSafePayPaymentCreatorOrderInterface {

  /**
   * The payment.
   *
   * @var Payment
   */
  protected $payment;

  /**
   * The payment method this creator is attached to.
   *
   * @var PaymentMethod
   */
  protected $payment_method;

  /**
   * The creator plugin's ID.
   *
   * @var string
   */
  protected $pluginId;

  /**
   * The creator plugin's definition.
   *
   * @var array
   */
  protected $pluginDefinition = array();

  /**
   * The creator plugin's configuration.
   *
   * @var array
   */
  protected $configuration = array();

  /**
   * {@inheritdoc}
   */
  public static function create(Payment $payment, PaymentMethod $payment_method, array $configuration, $plugin_id, array $plugin_definition) {
    // It seems there is no way to have "new static()"-like functionality in
    // PHP 5.2, so we have to use this workaround instead.
    $class = $plugin_definition['class'];
    return new $class($payment, $payment_method, $configuration, $plugin_id, $plugin_definition);
  }

  /**
   * Constructs a MultiSafePayPaymentCreatorOrderBase.
   *
   * @param \Payment $payment
   *    The payment to which this creator is attached.
   * @param \PaymentMethod $payment_method
   *    The PaymentMethod to which this creator is attached.
   * @param array $configuration
   *    An associative array containing the creator's configuration, if any.
   * @param $plugin_id
   *    The creator's plugin ID.
   * @param array $plugin_definition
   *    The creator plugin's definition.
   */
  public function __construct(Payment $payment, PaymentMethod $payment_method, array $configuration, $plugin_id, array $plugin_definition) {
    $this->pluginId = $plugin_id;
    $this->pluginDefinition = $plugin_definition;
    $this->payment = $payment;
    $this->payment_method = $payment_method;
    $this->setConfiguration($configuration);
  }

  /**
   * {@inheritdoc}
   */
  public function getPluginId() {
    return $this->pluginId;
  }

  /**
   * {@inheritdoc}
   */
  public function getPluginDefinition() {
    return $this->pluginDefinition;
  }

  /**
   * {@inheritdoc}
   */
  public function getPayment() {
    return $this->payment;
  }

  /**
   * {@inheritdoc}
   */
  public function getPaymentMethod() {
    return $this->payment_method;
  }

  /**
   * {@inheritdoc}
   */
  public function label() {
    return $this->pluginDefinition['label'];
  }

  /**
   * {@inheritdoc}
   */
  public function getDescription() {
    return isset($this->pluginDefinition['description']) ? $this->pluginDefinition['description'] : NULL;
  }

  /**
   * {@inheritdoc}
   */
  public function getConfiguration() {
    return $this->configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function setConfiguration(array $configuration) {
    $this->configuration = $configuration + $this->defaultConfiguration();
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, array &$form_state) {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function validateConfigurationForm(array $form, array &$form_state) {
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array $form, array &$form_state) {
    if (!empty($form_state['values'])) {
      $this->setConfiguration($form_state['values']);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function createOrder(array $context, Payment $payment) {

    // Variables.
    global $base_url;
    $amount = 0;

    // Sum amount total from line items.
    foreach ($payment->line_items as $line_item) {
      $amount += $line_item->amount;
    }

    // Amount must be in cents es.10,00euro => 1000
    $amount = (float) $amount;
    $amount = $amount * 100;

    // Return array order.
    return array(
      "type" => "redirect",
      "order_id" => $payment->pid,
      "currency" => $payment->currency_code,
      "amount" => $amount,
      "description" => $payment->description,
      "payment_options" => array(
        "notification_url" => $base_url . "/multisafepay/callback?payment_pid=" . $payment->pid,
        "redirect_url" => $base_url . "/multisafepay/callback/success?payment_pid=" . $payment->pid,
        "cancel_url" => $base_url . "/multisafepay/callback/cancelled?payment_pid=" . $payment->pid,
      )
    );
  }

  /**
   * {@inheritdoc}
   */
  public function receiveFeedback($transaction_id, Payment $payment, $status) {
    // Check if a redirect user or notification from server MultiSafePay.
    if (isset($status)) {

      // Build message feedback for user or set cancelled status for payment.
      switch ($status) {
        case 'success':
          drupal_set_message(t('His payment has been successfully completed.'), 'status');
          break;
        case 'cancelled':
          // Set Payment Status
          $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_CANCELLED));
          break;
      }

      // Log.
      watchdog(
        'msp_payment',
        t('User with payment \'@pid\' return from MultiSafepay with status \'@status\''),
        array(
          '@pid' => $payment->pid,
          '@status' => $status,
        ),
        WATCHDOG_INFO
      );

    }
    else {

      // Notification.

      // Get status of payment throuth call to MultiSafepay
      $msp_order = multisafepay_payment_get_order_from_multisafepay($transaction_id, $payment->method);
      $payment_status = $payment->method->controller->mapStatus($msp_order->status);

      // Set Payment Status
      $payment->setStatus(new PaymentStatusItem($payment_status));

      // Log
      watchdog(
        'msp_payment',
        t('Notification payment \'@pid\' return from MultiSafepay with status \'@status\''),
        array(
          '@pid' => $payment->pid,
          '@status' => $payment_status,
        ),
        WATCHDOG_INFO
      );
    }

    $payment->finish();
  }

}
