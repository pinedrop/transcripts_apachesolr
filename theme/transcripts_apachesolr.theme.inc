<?php

function theme_transcripts_search_snippet($vars) {
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
function theme_transcripts_goto_tcu($vars) {
        $mins = floor ($vars['element']['#time'] / 60);
        $secs = $vars['element']['#time'] % 60;
        $time = sprintf ("%d:%02d", $mins, $secs);

	$markup = variable_get('transcripts_ui_markup', 'default');
	if ($markup == 'bootstrap') {
                $out = "<a href='" . $vars['element']['#linkurl'] . "' class='btn btn-default btn-sm'>";
                $out .= "<span class='glyphicon glyphicon-play'></span> ";
                $out .= $time;
                $out .= "</a>";
	}
	else {
                $out = "<a href='" . $vars['element']['#linkurl'] . "'>";
                $out .= "<span class='ui-icon ui-icon-play ui-state-default'>Play</span>";
                $out .= "</a>";
	}
	return $out;
}
