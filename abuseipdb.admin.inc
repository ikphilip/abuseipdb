<?php

/**
 * @file
 * Developed by Gabor Szanto.
 *  hello@szantogabor.com
 *  http://szantogabor.com
 */

function abuseipdb_admin_general($form, &$form_state) {

  $form['abuseipdb_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Api key'),
    '#description' => t('The Api key provided by AbuseIPDB.'),
    '#default_value' => variable_get('abuseipdb_api_key', ''),
  );

  $form['abuseipdb_form_ids'] = array(
    '#type' => 'textarea',
    '#title' => t('Form ids'),
    '#description' => t('List of form ids to check before submission. One id per line!'),
    '#default_value' => variable_get('abuseipdb_form_ids', ''),
  );

  $form['abuseipdb_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Block AbuseIPDB positive ips'),
    '#description' => t("Add user's ip address to the blocked ip table, if it has AbuseIPDB record."),
    '#default_value' => variable_get('abuseipdb_block', FALSE),
  );

  $form['abuseipdb_emergency'] = array(
    '#type' => 'checkbox',
    '#title' => t('Emergency shutdown'),
    '#description' => t('Disable every AbusIPDB connection. Only cached data will be used.'),
    '#default_value' => variable_get('abuseipdb_emergency', FALSE),
  );

  return system_settings_form($form);
}
