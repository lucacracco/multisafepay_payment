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
  public function __construct($plugin_id, array $plugin_definition, array $configuration) {
    parent::__construct($plugin_id, $plugin_definition, $configuration);
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return parent::defaultConfiguration();
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, array &$form_state) {
    return parent::buildConfigurationForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array $form, array &$form_state) {
    parent::submitConfigurationForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function createOrder(Payment $payment) {
    return parent::createOrder($payment);
  }

  /**
   * {@inheritdoc}
   */
  public function receiveFeedback($transaction_id, Payment $payment, $status) {
    parent::receiveFeedback($transaction_id, $payment, $status);
  }
}
