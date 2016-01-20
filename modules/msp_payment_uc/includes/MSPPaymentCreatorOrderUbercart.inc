<?php

/**
 * @file MSPPaymentCreatorOrderUbercart.inc
 * Contains MSPPaymentCreatorOrderUbercart.
 */

/**
 * Provides a creator order plugin that used for Drupal Ubercart orders.
 */
class MSPPaymentCreatorOrderUbercart extends MultiSafePayPaymentCreatorOrderBase {

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return array(
      'test' => array(),
    );
  }

  /**
   * {@inheritdoc}
   */
  // TODO: finish this function after implemented form in form creation payment method.
  public function buildConfigurationForm(array $form, array &$form_state) {

    $form['test'] = array(
      '#type' => 'textfield',
      '#title' => t('Test'),
      '#description' => t('Description'),
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  // TODO: finish this function after implemented form in form creation payment method.s
  public function submitConfigurationForm(array $form, array &$form_state) {
    $values = $form_state['values'];
    $values['fields'] = array_keys(array_filter($values['fields']));
    $this->setConfiguration($values);
  }

  /**
   * {@inheritdoc}
   */
  public function createOrder(array $context, Payment $payment) {

    // Variables.
    global $base_url;

    if ($payment->context != "payment_ubercart") {

      // This payment is not reference a commerce order.
      watchdog(
        'msp_payment_commerce',
        t('This payment \'@pid\' is not reference a ubercart order'),
        array(
          '@pid' => $payment->pid,
        ),
        WATCHDOG_ERROR
      );
      return NULL;
    }

    // Load uc order.
    $order_id = $payment->payment_ubercart_uc_order_id;
    $uc_order = uc_order_load($order_id);
    $wrapper_uc_order = entity_metadata_wrapper('uc_order', $uc_order);

    // Total Order
    $amount = $wrapper_uc_order->order_total->value();

    // Amount must be in cents es.10,00euro => 1000
    // TODO: correct conversion because created problem to cast.
    $amount = (float) $amount;
    $amount = $amount * 100;

    // Return array order.
    $msp_order = array(
      "type" => "redirect",
      "order_id" => $order_id,
      "currency" => variable_get('uc_currency_code', 'EUR'),
      "amount" => $amount,
      "description" => $payment->description,
      "payment_options" => array(
        "notification_url" => $base_url . "/multisafepay/callback?payment_pid=" . $payment->pid,
        "redirect_url" => $base_url . "/multisafepay/callback/success?payment_pid=" . $payment->pid,
        "cancel_url" => $base_url . "/multisafepay/callback/cancelled?payment_pid=" . $payment->pid,
      )
    );

    // Customer billing
    if ($wrapper_uc_order->__isset('billing_address')) {

      // Customer billing.
      $customer_billing = $wrapper_uc_order->billing_address;
      $msp_order += array(

        // IMPORTANT: Without receiving customer data we are not able to do an external check on credit card transactions.
        "customer" => array(
//          "locale" => NULL,
//          "ip_address" => NULL,
//          "forwarded_ip" => NULL,
          "first_name" => $customer_billing->first_name->value(),
          "last_name" => $customer_billing->last_name->value(),
          "address1" => $customer_billing->street1->value(),
          "address2" => $customer_billing->street2->value(),
          "house_number" => NULL,
          "zip_code" => $customer_billing->postal_code->value(),
          "city" => $customer_billing->city->value(),
//          "state" => NULL,
          "country" => $customer_billing->country->value(),
          "phone" => $customer_billing->phone->value(),
          "email" => $customer_billing->email->value(),
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
