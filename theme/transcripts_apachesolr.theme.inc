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
