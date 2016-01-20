<?php

/**
 * @file MultiSafePayPaymentCreatorOrderRedirect.inc
 * Contains MultiSafePayPaymentCreatorOrderRedirect.
 */

/**
 * Provides a creator order plugin that used for normal orders.
 */
class MultiSafePayPaymentCreatorOrderRedirect extends MultiSafePayPaymentCreatorOrderBase {

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
    $amount = 0;

    // Sum amount total from line items.
    foreach ($payment->line_items as $line_item) {
      $amount += $line_item->amount;
    }

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

      // Build message feedback for user.
      $message = t('Error: feedback not correct');
      $type_message = 'error';
      switch ($status) {
        case 'success':
          $message = t('His payment has been successfully completed.');
          $type_message = 'status';
          break;
        case 'cancelled':
          $message = t('His payment has not been successful.');
          $type_message = 'warning';
          break;
      }

      // Print message and redirect to front page site.
      drupal_set_message($message, $type_message);
      drupal_goto('<front>');

    }
    else {

      // Notification.

      // Get status of payment throuth call to MultiSafepay
      $msp_order = multisafepay_payment_get_order_from_multisafepay($transaction_id, $payment->method);
      $payment_status = $payment->method->controller->mapStatus($msp_order->status);

      // Set Payment Statuss
      $payment->setStatus(new PaymentStatusItem($payment_status));
      entity_save('payment', $payment);

      $payment->finish();
    }

  }
}
