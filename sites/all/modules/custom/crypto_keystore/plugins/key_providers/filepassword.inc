<?php

/**
 * @file
 * Plugin definition for the FilePassword key provider.
 */

use \Defuse\Crypto\Crypto;

define('ENCRYPT_FILEPASSWORD_NAME', 'encrypt_password_key.key');

$plugin = crypto_keystore_encrypt_key_providers();

/**
 * Implements MODULE_FILENAME_encrypt_key_providers().
 */
function crypto_keystore_encrypt_key_providers() {
  return array(
    'title' => t('File with password'),
    'description' => t('Use a file to store the encryption key.'),
    'key callback' => 'crypto_keystore_get_file_key',
    'settings form' => 'crypto_keystore_file_key_settings_form',
    'submit callback' => 'crypto_keystore_file_key_settings_form_submit',
    'static key' => TRUE,
  );
}

/**
 * Callback method to return the encryption key from a file.
 */
function crypto_keystore_get_file_key($settings) {
  $file_name = $settings['path'] . '/' . ENCRYPT_FILEPASSWORD_NAME;
  // If the file doesn't exist, just abort.
  if (!file_exists($file_name)) {
    drupal_set_message(t('The file %file does not exist! Encrypt cannot retrieve encryption key.', array('%file' => $file_name)), 'error');
    return;
  }

  $key_crypt = file_get_contents($file_name);

  if($settings['password']){
    if (real_aes_load_library()) {
      $key = Crypto::decrypt($key_crypt, $settings['password']);
      return $key;
    }
  }
}

/**
 * Settings form for our key provider.
 */
function crypto_keystore_file_key_settings_form($defaults) {
  $form = array();

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to Key File'),
    '#description' => t('Filepath may be absolute (e.g. %abs) or relative to the Drupal directory (e.g. %rel).', array(
      '%abs' => '/var/www',
      '%rel' => '../../keys',
    )),
    '#default_value' => isset($defaults['path']) ? $defaults['path'] : '',
    '#element_validate' => array('crypto_keystore_file_key_path_validate'),
    '#required' => TRUE,
    '#field_suffix' => '/' . ENCRYPT_FILEPASSWORD_NAME,
  );

  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Key Password'),
    '#description' => t('Password to encrypt the key, must be 16 characters'),
    '#required' => TRUE,
    '#element_validate' => array('crypto_keystore_file_key_password_validate'),
  );

  return $form;
}

/**
 * Validate key path.
 */
function crypto_keystore_file_key_path_validate($element, &$form_state, $form) {
  $directory = $element['#value'];
  $file = $element['#value'] . '/' . ENCRYPT_FILEPASSWORD_NAME;

  if (!file_exists($directory)) {
    form_error($element, t('The directory %dir does not exist.', array('%dir' => $directory)));
  }
  elseif (!is_dir($directory)) {
    form_error($element, t('The key path provided exists, but is not a directory.'));
  }
  elseif (!file_exists($file) && !is_writable($directory)) {
    form_error($element, t('The directory %dir needs to be writable in order to create the key file.', array(
      '%dir' => $directory,
    )));
  }
  elseif (file_exists($file) && !is_readable($file)) {
    form_error($element, t('The key file %file exists, but is not readable by the web server.', array(
      '%file' => $file,
    )));
  }
}

/**
 * Validate callback for the settings form
 */

function crypto_keystore_file_key_password_validate($element, &$form_state, $form){
  $password = $element['#value'];
  if(strlen($password) != 16){
    form_error($element, t('The password length must be exactly 16 characters'));
  }
}

/**
 * Submit callback for the settings form.
 */
function crypto_keystore_file_key_settings_form_submit($form, $form_state) {
  $directory = $form_state['values']['encrypt_key_providers_filepassword_settings']['path'];
  $file = $directory . '/' . ENCRYPT_FILEPASSWORD_NAME;

  // Check for contents of file; do not write over the key.
  if (file_exists($file) && file_get_contents($file)) {
    return;
  }

  // Create a default key.
  $open_file = fopen($file, 'wb');

  $key_clear = drupal_random_bytes(16);

  //Using password to encrypt the key and save it to file
  if($form_state['values']['encrypt_key_providers_filepassword_settings']['password']){
    if (real_aes_load_library()) {
      $key = Crypto::encrypt($key_clear, $form_state['values']['encrypt_key_providers_filepassword_settings']['password']);
    }
  }

  fwrite($open_file, $key);
  fclose($open_file);

  // Ensure that the file is only readable and writable by owner.
  drupal_chmod($file, 0600);

  drupal_set_message(t('The key file has been written to %file', array(
    '%file' => $file,
  )));
}
