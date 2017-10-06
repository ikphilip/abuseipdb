<?php

/**
 * @file
 * Developed by Gabor Szanto.
 *  hello@szantogabor.com
 *  http://szantogabor.com
 */

/**
 * Form builder for general settings.
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

/**
 * Form builder to report an ip address to AbuseIPDB
 */
function abuseipdb_admin_report($form, &$form_state) {
  $form['ip'] = array(
    '#type' => 'textfield',
    '#title' => t('Ip address'),
    '#description' => t('The ip address to be reported.'),
    '#required' => TRUE,
  );

  $form['comment'] = array(
    '#type' => 'textarea',
    '#title' => t('Comment'),
    '#description' => t('Why do you want to report.'),
  );

  $map = abuseipdb_get_categories_mapping();
  $categories = array();
  $description = array();
  foreach ($map as $key => $value) {
    $categories[$key] = $value['title'];
    $description[] = $value['title'] . ': ' . $value['desc'];
  }

  $form['categories'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Categories'),
    '#options' => $categories,
    '#description' => theme('item_list', array('items' => $description)),
  );

  $form['block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Block ip'),
    '#description' => t('Block ip address after reported.'),
    '#default_value' => FALSE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Report to AbuseIPDB'),
  );

  return $form;
}

/**
 * Form validation before report an ip to AbuseIPDB.
 */
function abuseipdb_admin_report_validate($form, &$form_state) {
  if (!filter_var($form_state['values']['ip'], FILTER_VALIDATE_IP)) {
    form_set_error('ip', t('Please enter a valid ip address!'));
  }
}

function abuseipdb_admin_report_submit($form, &$form_state) {
  abuseipdb_report_ip($form_state['values']['ip'], $form_state['values']['categories'], $form_state['values']['comment']);

  if ($form_state['values']['block']) {
    $query = db_merge('blocked_ips')
      ->key(array('ip' => $form_state['values']['ip']))
      ->execute();
  }
}
