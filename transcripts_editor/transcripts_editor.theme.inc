<?php

function theme_transcripts_editor_tcu_gear($vars)
{
    $sid = $vars['element']['#sid'];
    $out  = "<button class='btn btn-default dropdown-toggle dropdown-menu-right' data-toggle='dropdown' type='button' id='transcript-segop-$sid'>";
    $out .= "<span class='glyphicon glyphicon-cog'></span>";
    $out .= "</button>";
    $out .= "<ul class='dropdown-menu'>";
    foreach ($vars['element']['#ops'] as $op) {
        $out .= "<li><a href='#'>$op</a></li>";
    }
    $out .= "</ul>";
    return $out;
}