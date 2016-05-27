<?php
/**
 * @file
 * Manager part of code for Billing client module.
 *
 */

 /**
 * Invoices.
 */
function billing_client_invoices_history() {
  $invoices = billing_client_get('invoices');
  $rows = $links = array();
  if(is_array($invoices)){
    foreach ($invoices as $invoice) {
      $status = billing_invoice_get_status($invoice['status']);
  
      $username = $invoice['name'];
      $account = user_load_by_name($invoice['name']);
      if($account) {
        $username = theme('username', array('account' => $account));
      }
  
      $row = array();
      $row[] = array('data' => format_date($invoice['created'], 'short'));
      $row[] = array('data' => l('#' . $invoice['id'], 'billing/invoices/' . $invoice['id']));
      $row[] = array('data' => $username);
      $row[] = array('data' => $invoice['description']);
      $row[] = array('data' => billing_format_amount($invoice['amount']));
      $row[] = array('data' => $status);
  
      $links['view'] = array(
            'title' => t('View'),
            'href' => "admin/reports/invoices/" . $invoice['id'] . "/view",
          );
  
      $row[] = array(
        'data' => array(
          '#theme' => 'links',
          '#links' => $links,
        ),
      );
      $rows[] = $row;
    }
  }

  $header = array(t('Date'), t('Invoice #'), t('User'), t('Description'), t('Amount'), t('Status'), t('Operations'));

  $admin_links['create_invoice'] = array(
    'title' => t('Create Invoice'),
    'href' => "admin/reports/invoices/add",
  );

  $output['admin_links'] = array(
    '#theme' => 'links',
    '#links' => $admin_links,
  );

  $output['pager_before'] = array(
    '#theme' => 'altpager',
  );
  $output['table'] = array(
    '#theme' => 'table__billing_balance_history',
    '#rows' => $rows,
    '#header' => $header,
    '#empty' => t('No transactions yet.'),
  );
  $output['pager_after'] = array(
    '#theme' => 'altpager',
  );
  return $output;
}

function billing_invoices_create_invoice($form, &$form_state) {

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Client'),
    '#size' => 30,
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#weight' => -1,
  );

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount'),
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Sync'),
  );

  return $form;
}

/**
 * Validate handler for billing_invoices_create_invoice.
 */
function billing_invoices_create_invoice_validate($form, &$form_state) {
  $remote_user = billing_client_get('user/:name', array(':name' => $form_state['values']['name']));
  if(!is_array($remote_user)) {
    form_set_error('name', t('User !name is not synced. Please sync it first.', array('!name' => $form_state['values']['name'])));
  }
}

/**
 * Submit handler for billing_invoices_create_invoice.
 */
function billing_invoices_create_invoice_submit($form, &$form_state) {
  $params = array(
    ':name' => $form_state['values']['name'],
    'amount' => $form_state['values']['amount'], 
    'title' => $form_state['values']['title'], 
    'description' => $form_state['values']['description'], 
  );
  $answer = billing_client_post('user/:name/invoice', $params);
  drupal_set_message('<pre>' . print_r($answer, true). '</pre>');
}
