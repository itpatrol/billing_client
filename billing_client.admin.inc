<?php
/**
 * @file
 * Admin part of code for Billing client module.
 *
 */

/**
 * Paths to the files.
 */
function billing_client_settings($form, &$form_state) {
  if(variable_get('billing_client_token')){
    $answer = billing_client_get('ping');
    if(isset($answer['message'])){
      drupal_set_message(t('API is online'));
    }
    else{
      drupal_set_message(t('API is offline'), 'error');
    }
  }
  $form['billing_client_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Token'),
    '#default_value' => variable_get('billing_client_token'),
  );

  $form['billing_client_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Billing API url'),
    '#default_value' => variable_get('billing_client_api_url'),
  );


  return system_settings_form($form);
}

/**
 * Manipulate user synchronisation.
 */
function billing_client_users($form, &$form_state) {
  $remote_users = billing_client_get('users');
  
  $synced_users = array();
  foreach($remote_users as $user) {
    $synced_users[$user['name']] = $user;
  }
  
  drupal_set_message('<pre>'. print_r($synced_users,true). '</pre>');
  
  $header = array(
    'username' => array('data' => t('Username'), 'field' => 'u.name'),
    'status' => array('data' => t('Status'), 'field' => 'u.status'),
    'roles' => array('data' => t('Roles')),
    'member_for' => array('data' => t('Member for'), 'field' => 'u.created', 'sort' => 'desc'),
    'access' => array('data' => t('Last access'), 'field' => 'u.access'),
  );
  $query = db_select('users', 'u');
  $query->condition('u.uid', 1, '>');
  user_build_filter_query($query);

  $count_query = clone $query;
  $count_query->addExpression('COUNT(u.uid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
  ->fields('u', array('uid', 'name', 'status', 'created', 'access'))
    ->limit(50)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();

  $status = array(t('blocked'), t('active'));
  $roles = array_map('check_plain', user_roles(TRUE));
  $accounts = array();
  
  foreach ($result as $account) {
    $users_roles = array();
    $accounts[$account->name] = $account; 
    $roles_result = db_query('SELECT rid FROM {users_roles} WHERE uid = :uid', array(':uid' => $account->uid));
    foreach ($roles_result as $user_role) {
      $users_roles[] = $roles[$user_role->rid];
    }
    asort($users_roles);
    
    $sync_status = t('Not synced');
    
    if(isset($synced_users[$account->name])) {
      $sync_status = t('Synced');
    }

    $options[$account->uid] = array(
      'username' => theme('username', array('account' => $account)),
      'status' => $status[$account->status] . ' / ' . $sync_status,
      'roles' => theme('item_list', array('items' => $users_roles)),
      'member_for' => format_interval(REQUEST_TIME - $account->created),
      'access' => $account->access ? t('@time ago', array('@time' => format_interval(REQUEST_TIME - $account->access))) : t('never'),
    );
  }
  
  $form_state['accounts'] = $accounts;

  $form['accounts'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No people available.'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Sync'),
  );
  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}

/**
 * Submit handler for billing_client_users.
 */
function billing_client_users_submit($form, &$form_state) {
  $accounts = array_filter($form_state['values']['accounts']);
  drupal_set_message('<pre>'. print_r($form_state['values']['accounts'],true). '</pre>');
  drupal_set_message('<pre>'. print_r($form_state['accounts'],true). '</pre>');
}