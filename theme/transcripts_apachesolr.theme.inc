<?php

function theme_transcripts_apachesolr_search_snippet($vars)
{
  $out = "<div class='controls'>";
  $out .= drupal_render($vars['element']['link']);
  $out .= "</div>";
  $out .= "<div class='tiers'>" . implode('', $vars['element']['tiers']) . "</div>";
  return $out;
}

function theme_transcripts_apachesolr_html5_audio($vars)
{
  $out = "<div data-transcripts-role='video' data-transcripts-id='{$vars['trid']}'>";
  $out .= "<audio class='embed-responsive-item' controls>";
  foreach ($vars['urls'] as $url) {
    $out .= "<source src='$url' />";
  }
  $out .= "</audio>";
  $out .= "</div>";
  return $out;
}

function theme_transcripts_apachesolr_html5_video($vars)
{
  $out = "<div data-transcripts-role='video' data-transcripts-id='{$vars['trid']}'>";
  $out .= "<div align='center' class='embed-responsive embed-responsive-4by3'>";
  $out .= "<video class='embed-responsive-item' controls>";
  foreach ($vars['urls'] as $url) {
    $out .= "<source src='$url' />";
  }
  $out .= "</video>";
  $out .= "</div>";
  $out .= "</div>";
  return $out;
}

function theme_transcripts_apachesolr_link_tcu($vars)
{
  $minutes = floor($vars['element']['#time'] / 60);
  $secs = $vars['element']['#time'] % 60;
  $time = sprintf("%d:%02d", $minutes, $secs);
  $classes = 'btn btn-default';
  $classes .= $vars['element']['#timecoded'] ? ' timed' : ' untimed';
  $out = "<a href='" . $vars['element']['#linkurl'] . "' class='" . $classes . "' role='button'>";
  $out .= "<span class='glyphicon glyphicon-play'></span> ";
  $out .= $time;
  if (isset($vars['element']['#text'])) {
    $out .= " " . $vars['element']['#text'];
  }
  $out .= "</a>";
  return $out;
}

function theme_transcripts_apachesolr_tier_label($vars)
{
  $output = '<span class="language-name">' .$vars['language_name']. '</span>';
  if ($vars['language_code']) {
    $output .= ' (<span class="language-code">' .$vars['language_code']. '</span>)';
  }
  return $output;
}
