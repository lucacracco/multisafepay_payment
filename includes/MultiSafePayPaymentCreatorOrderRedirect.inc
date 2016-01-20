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
    $msp_order = parent::createOrder($context, $payment);
    return $msp_order;
  }

  /**
   * {@inheritdoc}
   */
  public function receiveFeedback($transaction_id, Payment $payment, $status) {
    parent::receiveFeedback($transaction_id, $payment, $status);
  }
}
