# multisafepay_payment
Provides an MultiSafePay payment method type for Drupal 7 Payment

You need to download the Multisafepay library before enable the module.

Method 1: 
1. Item download https://github.com/MultiSafepay/PHP/archive/master.zip and unzip in  sites/all/libraries folder
1. Item rename "PHP-master" folder in "multisafepay_php"

Method 2: from the root dir of your project in CLI type:
```
git subtree add --prefix sites/all/libraries/multisafepay_php https://github.com/MultiSafepay/PHP.git master --squash
```

# After enabling module task:
clear all cache and run manually cron: this helps the Rules module to rebuild the needed rules

# Theming:
TODO
