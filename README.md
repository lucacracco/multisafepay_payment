# multisafepay_payment
Provides an MultiSafePay payment method type for Drupal 7 Payment. It integrates with Drupal Commerce and Ubercart 3.

You need to download the Multisafepay library before enable the module.

Method 1: 
 1. download https://github.com/MultiSafepay/PHP/archive/master.zip and unzip in  sites/all/libraries folder
 2. rename "PHP-master" folder in "multisafepay_php"

Method 2: from the root dir of your project in CLI type:
```
git subtree add --prefix sites/all/libraries/multisafepay_php https://github.com/MultiSafepay/PHP.git master --squash
```

# Before enable module
Ensure that [Payment](https://www.drupal.org/project/payment) module is present in your installation.
If you want to use it with Drupal Commerce you need also [Payment for Drupal Commerce](https://www.drupal.org/project/payment_commerce) otherwise you have to enable [Payment for Ubercart](https://www.drupal.org/project/payment_ubercart)

# After enabling module task:
clear all cache and run manually cron: this helps the Rules module to rebuild the needed rules

# Theming:
TODO
