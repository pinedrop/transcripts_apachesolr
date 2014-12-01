<?php

function theme_transcripts_apachesolr_search_snippet($vars) {
        $markup = variable_get('transcripts_ui_markup', 'default');
        if ($markup == 'bootstrap') {
                $out = "<div class='controls'>";
		$out .= drupal_render($vars['element']['link']);
                $out .= "</div>";
		$out .= "<div class='tiers'>" . implode('', $vars['element']['tiers']) . "</div>";
	}
        else {
		$out = "<div class='controls'>";
                $out .= drupal_render($vars['element']['link']);
		$out .= "</div>";
		$out .= "<div class='tiers'>" . implode('', $vars['element']['tiers']) . "</div>";
	}
	return $out;
}
function theme_transcripts_apachesolr_video_tag($vars) {
        $out = "<div class='transcripts-apachesolr-video-tag' data-trid='{$vars['trid']}'>";
        if (!$vars['video_tag']) { //use video_url
                $tag = $vars['tag_type'];
                $out .= "<div class='transcripts'>";
                $out .= "<div class='transcripts-apachesolr-html5-media'>";
                $out .= "<$tag controls>";
                foreach ($vars['video_url'] as $vurl) {
                        $out .= "<source src='$vurl' />";
                        //$out .= "<source type='video/$src->type' src='$src->src' width='$src->width' height='$src->height' />";
                }
                $out .= "</$tag>";
                $out .= "</div>";
                $out .= "</div>";
        }
        else {
                $out .= $vars['video_tag'];
        }
        $out .= "</div>";
        return $out;
}
