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
   * The suggester plugin's ID.
   *
   * @var string
   */
  protected $pluginId;

  /**
   * The suggester plugin's definition.
   *
   * @var array
   */
  protected $pluginDefinition = array();

  /**
   * The suggester plugin's configuration.
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
   * Constructs a SearchApiAutocompleteSuggesterPluginBase.
   *
   * @param Payment $payment
   *   The payment to which this creator is attached.
   * @param array $configuration
   *   An associative array containing the suggester's configuration, if any.
   * @param array $configuration
   *   An associative array containing the suggester's configuration, if any.
   * @param string $plugin_id
   *   The suggester's plugin ID.
   * @param array $plugin_definition
   *   The suggester plugin's definition.
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

//  public function createOrder() {
//    // TODO: Implement createOrder() method.
//  }

}
