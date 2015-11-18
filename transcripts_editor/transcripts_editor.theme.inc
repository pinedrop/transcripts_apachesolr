<?php

function theme_transcripts_editor_tcu_gear($vars)
{
    $out  = '<div class="dropdown">';
    $out .= '<button class="tcu-gear-ops btn btn-default dropdown-toggle dropdown-menu-right" type="button" id="tcu-gear-ops-' .$vars['element']['#tcuid']. '" data-toggle="dropdown">';
    $out .= '<span class="glyphicon glyphicon-cog"></span>';
    $out .= '</button>';
    $out .= '<ul class="dropdown-menu">';
    foreach ($vars['element']['#actions'] as $key => $name) {
        $out .= '<li><a data-val="' .$key. '" href="#">' .$name. '</a></li>';
    }
    $out .= '</ul>';
    $out .= '</div>';
    return $out;
}