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
    $row[] = array('data' => billing_format_amount($invoice['amount']) . $invoice['amount']);
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

  $header = array(t('Date'), t('Invoice #'), t('User'), t('Description'), t('Amount'), t('Status'), t('Operations'));

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