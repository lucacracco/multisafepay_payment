<?php

/**
 * @file MSPPaymentCreatorOrderCommerce.inc
 * Contains MSPPaymentCreatorOrderCommerce.
 */

/**
 * Provides a creator order plugin that used for Drupal Commerce orders.
 */
class MSPPaymentCreatorOrderCommerce extends MultiSafePayPaymentCreatorOrderBase {

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return parent::defaultConfiguration();
  }

  /**
   * {@inheritdoc}
   */
  // TODO: finish this function after implemented form in form creation payment method.
  public function buildConfigurationForm(array $form, array &$form_state) {
    return parent::buildConfigurationForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  // TODO: finish this function after implemented form in form creation payment method.s
  public function submitConfigurationForm(array $form, array &$form_state) {
    parent::submitConfigurationForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function createOrder(Payment $payment) {

    // Variables.
    global $base_url;

    if ($payment->context != "payment_commerce") {

      // This payment is not reference a commerce order.
      watchdog(
        'msp_payment_commerce',
        t('This payment \'@pid\' is not reference a commerce order'),
        array(
          '@pid' => $payment->pid,
        ),
        WATCHDOG_ERROR
      );
      return NULL;
    }

    // Load commerce order.
    $commerce_order = commerce_order_load($payment->context['order_id']);
    $wrapper_commerce_order = entity_metadata_wrapper('commerce_order', $commerce_order);

    // Return array order.
    $msp_order = array(
      "type" => "redirect",
      "order_id" => $payment->context['order_id'],
      "currency" => $payment->currency_code,
      "amount" => $payment->context['balance_amount'],
      "description" => $payment->description,
      "payment_options" => array(
        "notification_url" => $base_url . "/multisafepay/callback?payment_pid=" . $payment->pid,
        "redirect_url" => $base_url . "/multisafepay/callback/success?payment_pid=" . $payment->pid,
        "cancel_url" => $base_url . "/multisafepay/callback/cancelled?payment_pid=" . $payment->pid,
      )
    );

    // Customer billing
    if ($wrapper_commerce_order->__isset('commerce_customer_billing')) {

      // Customer billing.
      $customer_billing = $wrapper_commerce_order->commerce_customer_billing;
      $msp_order += array(

        // IMPORTANT: Without receiving customer data we are not able to do an external check on credit card transactions.
        "customer" => array(
//          "locale" => NULL,
//          "ip_address" => NULL,
//          "forwarded_ip" => NULL,
          "first_name" => $customer_billing->commerce_customer_address->first_name->value(),
          "last_name" => $customer_billing->commerce_customer_address->last_name->value(),
          "address1" => $customer_billing->commerce_customer_address->thoroughfare->value(),
          "address2" => $customer_billing->commerce_customer_address->premise->value(),
          "house_number" => NULL,
          "zip_code" => $customer_billing->commerce_customer_address->postal_code->value(),
          "city" => $customer_billing->commerce_customer_address->locality->value(),
//          "state" => NULL,
          "country" => $customer_billing->commerce_customer_address->country->value(),
          "phone" => NULL,
          "email" => $wrapper_commerce_order->mail->value(),
          // True if you will send your own bank transfer payment instructions to consumers and do not want MultiSafepay to do this.
          "disable_send_email" => NULL,
        ),
      );
    }

    return $msp_order;
  }

  /**
   * {@inheritdoc}
   */
  public function receiveFeedback($transaction_id, Payment $payment, $status) {
    parent::receiveFeedback($transaction_id, $payment, $status);
  }
}
