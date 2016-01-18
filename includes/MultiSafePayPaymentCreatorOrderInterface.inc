<?php

/**
 * @file
 * Contains MultiSafePayPaymentCreatorOrderInterface.
 */

/**
 * Represents a plugin for creating 'creator' order for send to server MultiSafepay.
 */
interface MultiSafePayPaymentCreatorOrderInterface {

  /**
   * Creates a new instance of this class.
   *
   * @param \Payment $payment
   *   Payment transaction.
   * @param \PaymentMethod $payment_method
   *   Payment Method reference transaction.
   * @param array $configuration
   *   An associative array containing the suggester's configuration, if any.
   * @param string $plugin_id
   *   The suggester's plugin ID.
   * @param array $plugin_definition
   *   The suggester plugin's definition.
   * @return static A new instance of this class.
   *   A new array order data to send a server MultiSafepay.
   */
  public static function create(Payment $payment, PaymentMethod $payment_method, array $configuration, $plugin_id, array $plugin_definition);

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
   * Retrieves the payment.
   *
   * @return Payment
   *   The payment reference.
   */
  public function getPayment();

  /**
   * Retrieves the payment method.
   *
   * @return PaymentMethod
   *   The payment method reference.
   */
  public function getPaymentMethod();

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
   * @return mixed
   */
  public function createOrder();

}
