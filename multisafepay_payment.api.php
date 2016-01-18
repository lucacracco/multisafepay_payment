<?php

/**
 * @file multisafepay_payment.api.php
 */

/**
 * Alter data MultiSafePay Order.
 *
 * @param array $msp_order
 * @param \Payment $payment
 *
 * @return NULL
 */
function hook_multisafepay_payment_msp_order_msp_order(array &$msp_order, Payment $payment) {

  // This is example alteration.
  // Full variable we found in documentation MultiSafepay.
  // https://www.multisafepay.com/documentation/doc/API-Reference/

  // Connect.
  if ($msp_order['type'] == 'redirect') {

    $msp_order = array(
      // Specifies the payment flow for the checkout process. For normal orders use the value "redirect". (required)
//      "type" => "redirect",
      // The unique identifier from your system for the order. (required)
      "order_id" => NULL,
      // The currency you want the customer to pay with. (required)
      "currency" => NULL,
      // The amount (in cents) that the customer needs to pay. (required)
      "amount" => NULL,
      // The unique gateway id to immediately direct the customer to the payment method.
      "gateway" => NULL,
      // A free text description which will be shown with the order in MultiSafepay Control.
      "description" => NULL,
      // var1, var2, var3: optional.
      // A free variable for custom data to be stored and persisted.
      "var1" => NULL,
      "var2" => NULL,
      "var3" => NULL,
      // Include an HTML formatted list of the items ordered. Used to customer how the order is displayed on payment pages and emails.
      "items" => NULL,
      // If true this forces a credit card transaction to require manual acceptance regardless of the outcome from fraud checks.
      "manual" => NULL,
      // The number of days the payment link will be active for. Default is 30 days.
      "days_active" => NULL,
      //
      "payment_options" => array(
        // The URL that MultiSafepay will send transaction status updates to when the status of a transaction changes. (required)
        "notification_url" => '/',
        // Where we will direct a customer from our payment pages after a successful transaction. (required)
        "redirect_url" => '/',
        // Where we will direct a customer from our payment pages after a cancelled or unsuccessful transaction. (required)
        "cancel_url" => '/',
        // Set to true if you will display the MultiSafepay payment page in a new window and want it to close automatically after the payment process has been completed.
        "close_window" => TRUE,
      ),
      // IMPORTANT: Without receiving customer data we are not able to do an external check on credit card transactions.
      "customer" => array(
        "locale" => NULL,
        "ip_address" => NULL,
        "forwarded_ip" => NULL,
        "first_name" => NULL,
        "last_name" => NULL,
        "address1" => NULL,
        "address2" => NULL,
        "house_number" => NULL,
        "zip_code" => NULL,
        "city" => NULL,
        "state" => NULL,
        "country" => NULL,
        "phone" => NULL,
        "email" => NULL,
        // True if you will send your own bank transfer payment instructions to consumers and do not want MultiSafepay to do this.
        "disable_send_email" => NULL,
      ),
    );
  }

  // Fast Checkout.
  if ($msp_order['type'] == 'checkout') {
    $msp_order = array(
      // Specifies the payment flow for the checkout process. For Pay After Delivery orders use the value "checkout". (required)
//      "type" => "checkout",
      // Sets a predefined payment method.
      "gateway" => NULL,
      // The unique identifier from your system for the order. (required)
      "order_id" => NULL,
      // The currency you want the customer to pay with. (required)
      "currency" => NULL,
      // The unique identifier from your system for the order. (required)
      "amount" => NULL,
      // A free text description which will be shown with the order in MultiSafepay Control. (required)
      "description" => NULL,
      // A free variable for custom data to be stored and persisted.
      "var1" => NULL,
      "var2" => NULL,
      "var3" => NULL,
      // Include an HTML formatted list of the items ordered.
      "items" => NULL,
      // If true this forces a credit card transaction to require manual acceptance regardless of the outcome from fraud checks.
      "manual" => NULL,
      //
      "payment_options" => array(
        // The URL that MultiSafepay will send transaction status updates to when the status of a transaction changes. (required)
        "notification_url" => '/',
        // Where we will direct a customer from our payment pages after a successful transaction. (required)
        "redirect_url" => '/',
        // Where we will direct a customer from our payment pages after a cancelled or unsuccessful transaction. (required)
        "cancel_url" => '/',
        // Set to true if you will display the MultiSafepay payment page in a new window and want it to close automatically after the payment process has been completed.
        "close_window" => TRUE,
      ),
      // IMPORTANT: Without receiving customer data we are not able to do an external check on credit card transactions.
      "customer" => array(
        "locale" => NULL,
        "ip_address" => NULL,
        "forwarded_ip" => NULL,
        "first_name" => NULL,
        "last_name" => NULL,
        "address1" => NULL,
        "address2" => NULL,
        "house_number" => NULL,
        "zip_code" => NULL,
        "city" => NULL,
        "state" => NULL,
        "country" => NULL,
        "phone" => NULL,
        "email" => NULL,
        "referrer" => NULL,
        "user_agent" => NULL
      ),
      "delivery" => array(
        "first_name" => NULL,
        "last_name" => NULL,
        "address1" => NULL,
        "address2" => NULL,
        "house_number" => NULL,
        "zip_code" => NULL,
        "city" => NULL,
        "state" => NULL,
        "country" => NULL,
        "phone" => NULL,
        "email" => NULL
      ),
      "gateway_info" => array(
        "birthday" => NULL,
        "bank_account" => NULL,
        "phone" => NULL,
        "email" => NULL,
        "referrer" => NULL,
        "user_agent" => NULL
      ),
      "shopping_cart" => array(
        "items" => array(
          array(
            // The name of the ordered product. (required)
            "name" => NULL,
            // More detailed description of the product. (required)
            "description" => NULL,
            // The (tax exclusive) price per unit of the ordered product. (required)
            "unit_price" => NULL,
            // The currency for the unit_price of this product. All currencies must be the same as the order currency. (required)
            "currency" => NULL,
            // The number of this particular item ordered. (required)
            "quantity" => NULL,
            // Your unique identifier for this product. (required)
            "merchant_item_id" => NULL,
            // The name of the provided tax table MultiSafepay should when calculating tax on this product. (required)
            "tax_table_selector" => NULL,
            "weight" => array(
              // The units that the weight metric has been provided in.
              "unit" => NULL,
              // The number of units each of these items weighs.
              "value" => NULL
            )
          )
        )
      ),
      "checkout_options" => array(
        "rounding_policy" => array(
          "mode" => NULL,
          "rule" => NULL
        ),
        "shipping_methods" => array(
          //
          "flat_rate_shipping" => array(
            array(
              // The name of the pickup location. (required)
              "name" => NULL,
              // The price charged to the customer for using this shipping method. (required)
              "price" => NULL,
              // The currency that this shipping method will be charged in. (required)
              "currency" => NULL,
              "allowed_areas" => array(
                "NL",
                "ES"
              )
            ),
            array(
              "name" => NULL,
              "price" => NULL,
              "excluded_areas" => array(
                "NL",
                "FR",
                "ES"
              )
            )
          )
        ),
        "tax_tables" => array(
          "default" => array(
            // Indicates whether or tax will be charged on the cost of shipping. (required)
            "shipping_taxed" => NULL,
            // The rate of tax to charge on items this rule is applied to in decimal format. e.g. 0.21 (required)
            "rate" => NULL
          ),
          "alternate" => array(
            array(
              "name" => NULL,
              "rules" => array(
                array(
                  "rate" => NULL,
                  "country" => NULL
                )
              )
            )
          )
        ),
        "custom_fields" => array(
          array(
            "name" => NULL,
            "type" => NULL,
            "label" => NULL,
            "description_right" => array(
              "value" => array(
                array(
                  "nl" => NULL
                ),
                array(
                  "en" => NULL
                )
              )
            ),
            "validation" => array(
              "type" => "regex",
              "data" => "^array(1)$",
              "error" => array(
                array(
                  "nl" => NULL
                ),
                array(
                  "en" => NULL
                )
              )
            )
          ),
          array(
            "standard_type" => "companyname"
          ),
          array(
            "standard_type" => "passportnumber",
            "validation" => array(
              "type" => NULL,
              "data" => NULL,
              "error" => NULL
            )
          )
        )
      ),
    );
  }
}