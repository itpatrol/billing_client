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
function theme_billing_format_amount($price, $symbol, $position = FALSE) {
  $output .= print_r($price,true);
  if ($position) {
    $output .= '<div class="price"><span class="symbol">' . $symbol . '</span> ' . $price . '</div>';
  }
  else {
    $output .= '<div class="price">' . $price . '<span class="symbol">' . $symbol . '</span></div>';
  }
  return $output;
}
