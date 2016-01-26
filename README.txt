CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Troubleshooting
 * FAQ
 * Maintainers

INTRODUCTION
------------

The MultiSafepay Payment module allows the interaction with payment
systems provided by MultiSafepay. In addition to defining two submodules
that integrate with Drupal Commerce and Ubercart, it provides a simple
infrastructure to define the plugin associated with the payment method,
to be used to construct the vector data to send to server MultiSafepay
and to manage the payment notification or redirects the user to the site
from the payment pages of MultiSafepay.

 * For a full description of the module, visit the project page:
   https://github.com/lucacracco/multisafepay_payment

 * To submit bug reports and feature suggestions, or to track changes:
   https://github.com/lucacracco/multisafepay_payment/issues

REQUIREMENTS
------------

This module requires the following modules:

 * Payment (https://www.drupal.org/project/payment) >=1.8
 * Libraries API (https://www.drupal.org/project/libraries)
 * Entity API (https://www.drupal.org/project/entity)

For submodule MultiSafepay Payment Commerce:

 * Payment for Drupal Commerce (https://www.drupal.org/project/payment_commerce)

For submodule MultiSafepay Payment Ubercart:

 * Payment for Ubercart (https://www.drupal.org/project/payment_ubercart)


RECOMMENDED MODULES
-------------------

Nothing.

INSTALLATION
------------

 * Download libraries 'MultiSafepay PHP' from https://github.com/MultiSafepay/PHP.
   Extract zip and rename folder in 'multisafepay'. Copy libraries folder in
   drupal libraries folder (es. /sites/all/libraries/).
   At the end you should have /sites/all/libraries/multisafepay/models/API/Object/Core.php
   for example.

 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

 * Creates the payment method following the instructions of module Payment.

CONFIGURATION
-------------

 * No global configuration.
 Any method of payment mandatory to insert the API key and
 the choice of server usage (Test|Production).
 Other configurations are defined by plugins chosen or made.

 * See documentation of module Payment for create a new payment method.

TROUBLESHOOTING
---------------



FAQ
---

Q: What is the use of the plugin?

A: The implementation of the plugin allows you to define a plug that fits your needs.
   You can then customize the management of the data sent to the server MultiSafepay
   without using hooks or "rounds absurd" to change or implement customizations.
   Also in the plugin, you can also define the management of return notification
   from the server MultiSafepay.
   It is hoped that by using this infrastructure makes the customization,
   when required, more facilitated.

Q: How can I create my own plugin?

A: You have to create a class that extends the interface MultiSafePayPaymentCreatorOrderInterface,
   implement the methods required by the class
   (see example of how the base class MultiSafePayPaymentCreatorOrderBase
   and MultiSafePayPaymentCreatorOrderRedirect as derived).
   Make known to form your class by implementing the coupling
   hook_multisafepay_payment_plugin_msp_order_info() defining the required data.
   Once followed these steps the plugin will be selected
   in the configuration of the payment method.

Q: I have to do a little customization, I have to create a plugin the same?

A: Not necessarily. You can use a plugin already provided and the hook
   hook_multisafepay_payment_msp_order().

MAINTAINERS
-----------

Current maintainers:
 * Luca Cracco (luca_cracco) - https://www.drupal.org/u/luca_cracco
 * Emanuel Righetto () - https://www.drupal.org/u/emanuelrighetto

This project has been sponsored by:
 * AthenaSPA
   Athena is an International group with three divisions and ten plants both
   in Italy and around the world.
   Athena Industries: applications and automotive supplies, earth movement
   and agricultural machinery, food industry, adaptors, compressors,
   heat and cold machinery.
   Athena Parts: motorbike, scooter, maxi scooter, off road and automotive
   mechanics and electronics.
   Athena Evolution: Italian distribution of brands such as GoPro, VR|46,
   Red Bull Eyewear, Ogio, Skullcandy, TomTom, XSories, Jawbone, SP Gadgets,
   Go4Fun and Klaxon.