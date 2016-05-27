<?php

/**
 * Theme function for formating price with html
 *
 * @param  $price
 *   Price to format
 * @param  $symbol
 *   Symbol of currency
 * @param bool $position
 *   Position of price and it symbol
 * @return string
 *   Formated price
 */
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
