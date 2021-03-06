<?php

/**
 * @file
 * Contains MultiSafePayPaymentCreatorOrderInterface.
 */

/**
 * Represents a plugin for creating 'creator' order for send to server MultiSafepay
 * and receive feedback or notification.
 */
interface MultiSafePayPaymentCreatorOrderInterface {

  /**
   * Retrieves the plugin's ID.
   *
   * @return string
   *   The plugin's ID.
   */
  public function getPluginId();

  /**
   * Retrieves the plugin's definition.
   *
   * @return array
   *   The plugin's definition.
   */
  public function getPluginDefinition();

  /**
   * Retrieves the plugin's label.
   *
   * @return string
   *   The plugin's human-readable and translated label.
   */
  public function label();

  /**
   * Retrieves the plugin's description.
   *
   * @return string|null
   *   The plugin's translated description; or NULL if it has none.
   */
  public function getDescription();

  /**
   * Retrieves the plugin's configuration.
   *
   * @return array
   *   An associative array containing the plugin's configuration.
   */
  public function getConfiguration();

  /**
   * Sets the plugin's configuration.
   *
   * @param array $configuration
   *   An associative array containing the plugin's configuration.
   *
   * @return $this
   */
  public function setConfiguration(array $configuration);

  /**
   * Retrieves the default configuration for this plugin.
   *
   * @return array
   *   An associative array containing the plugin's default configuration.
   */
  public function defaultConfiguration();

  /**
   * Constructs the plugin's configuration form.
   *
   * @param array $form
   *   An associative array containing the structure of the form.
   * @param array $form_state
   *   The current state of the form.
   *
   * @return array
   *   An associative array containing the structure of the form. An empty array
   *   if the plugin has no configuration form.
   */
  public function buildConfigurationForm(array $form, array &$form_state);

  /**
   * Validates the plugin's configuration form.
   *
   * @param array $form
   *   An associative array containing the structure of the form.
   * @param array $form_state
   *   The current state of the form.
   */
  public function validateConfigurationForm(array $form, array &$form_state);

  /**
   * Submits the plugin's configuration form.
   *
   * Should take care of calling setConfiguration() with the new configuration
   * values as appropriate.
   *
   * @param array $form
   *   An associative array containing the structure of the form.
   * @param array $form_state
   *   The current state of the form.
   */
  public function submitConfigurationForm(array $form, array &$form_state);

  /**
   * Function to used for create order to send a MultiSafepay.
   *
   * @param \Payment $payment
   * @return mixed
   */
  public function createOrder(Payment $payment);

  /**
   * Function to used for feedback/notification transaction status or redirect buyer.
   *
   * @param $transaction_id
   *    Transaction ID
   * @param \Payment $payment
   *    Payment
   * @param $status
   *    Status receveid. Used when buyer is redirect to site from MultiSafepay
   * @return mixed
   *    NULL
   */
  public function receiveFeedback($transaction_id, Payment $payment, $status);
}
