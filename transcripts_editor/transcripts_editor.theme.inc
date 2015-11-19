<?php

function theme_transcripts_editor_tcu_gear($vars)
{
    $content  = '<span class="icon shanticon-close2"></span>';
    $content .= '<ul>';
    foreach ($vars['element']['#actions'] as $key => $name) {
        $content .= '<li><a class="tcu-action-link" data-tcuid="' . $vars['element']['#tcuid'] . '" data-val="' . $key . '" href="#">' . $name . '</a></li>';
    }
    $content .= '</ul>';

    $out = '<button id="tcu-gear-ops-' . $vars['element']['#tcuid'] . '" type="button" class="tcu-gear-ops btn btn-default" '
        .'data-toggle="popover" data-html="true" data-placement="bottom" '
        .'data-content="' .htmlspecialchars($content). '">'
        .'<span class="glyphicon glyphicon-cog"></span>'
        .'</button>';
    return $out;
}