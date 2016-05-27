<?php
  

function theme_billing_format_amount($variables) {
  $output = '';
  if ($variables['position']) {
    $output .= '<div class="price"><span class="symbol">' . $variables['symbol'] . '</span> ' . $variables['amount'] . '</div>';
  }
  else {
    $output .= '<div class="price">' . $variables['amount'] . '<span class="symbol">' . $variables['symbol'] . '</span></div>';
  }
  return $output;
}
