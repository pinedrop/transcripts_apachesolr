<?php

function theme_transcripts_apachesolr_search_snippet($vars)
{
    $out = "<div class='controls'>";
    $out .= drupal_render($vars['element']['link']);
    $out .= "</div>";
    $out .= "<div class='tiers'>" . implode('', $vars['element']['tiers']) . "</div>";
    return $out;
}

function theme_transcripts_apachesolr_video_tag($vars)
{
    $out = "<div data-transcripts-role='video' data-transcripts-id='{$vars['trid']}'>";
    if (!$vars['video_tag']) { //use video_url
        $tag = $vars['tag_type'];
        $out .= "<div class='transcripts'>";
        $out .= "<div class='transcripts-apachesolr-html5-media embed-responsive embed-responsive-16by9'>";
        $out .= "<$tag class='embed-responsive-item' controls>";
        foreach ($vars['video_url'] as $vurl) {
            $out .= "<source src='$vurl' />";
            //$out .= "<source type='video/$src->type' src='$src->src' width='$src->width' height='$src->height' />";
        }
        $out .= "</$tag>";
        $out .= "</div>";
        $out .= "</div>";
    } else {
        $out .= $vars['video_tag'];
    }
    $out .= "</div>";
    return $out;
}

function theme_transcripts_apachesolr_link_tcu($vars) {
    $minutes = floor ($vars['element']['#time'] / 60);
    $secs = $vars['element']['#time'] % 60;
    $time = sprintf ("%d:%02d", $minutes, $secs);
    $classes = 'btn btn-default';
    $classes .= $vars['element']['#timecoded'] ? ' timed' : ' untimed';
    $out = "<a href='" . $vars['element']['#linkurl'] . "' class='" . $classes ."' role='button'>";
    $out .= "<span class='glyphicon glyphicon-play'></span> ";
    $out .= $time;
    if (isset($vars['element']['#text'])) {
        $out .= " " . $vars['element']['#text'];
    }
    $out .= "</a>";
    return $out;
}