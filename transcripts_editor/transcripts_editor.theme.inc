<?php

function theme_transcripts_editor_tcu_gear($vars)
{
    $menu = '<ul class="dropdown-menu">';
    foreach ($vars['element']['#actions'] as $key => $name) {
        $menu .= '<li><a class="tcu-action-link" data-tcuid="' . $vars['element']['#tcuid'] . '" data-val="' . $key . '" href="#">' . $name . '</a></li>';
    }
    $menu .= '</ul>';

    $button = '<button id="tcu-gear-ops-' . $vars['element']['#tcuid'] . '" '
        .'class="tcu-gear-ops btn btn-default dropdown-toggle dropdown-menu-right" '
        .'type="button" data-toggle="dropdown">'
        .'<span class="glyphicon glyphicon-cog"></span>'
        .'</button>';

    return '<div class="dropdown">' .$button.$menu. '</div>';
}