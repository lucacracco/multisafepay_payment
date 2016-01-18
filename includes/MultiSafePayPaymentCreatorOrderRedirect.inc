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
  public function submitConfigurationForm(array $form, array &$form_state) {
    $values = $form_state['values'];
    $values['fields'] = array_keys(array_filter($values['fields']));
    $this->setConfiguration($values);
  }

  /**
   * {@inheritdoc}
   */
  public function createOrder() {

    // TODO: Implement createOrder() method.
    global $base_url;

    // TODO: for first test.
    return array(
      "type" => "redirect",
      "order_id" => time(),
      "currency" => "EUR",
      "amount" => "1000",
      "description" => "Demo",
      "payment_options" => array(
        "notification_url" => $base_url . "/node/2",
        "redirect_url" => $base_url . "/node/2",
        "cancel_url" => $base_url . "/node/2"
      )
    );
  }
}
