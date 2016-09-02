<?php

function hook_transcripts_editor_disabled($node) {
    if ($node->type == 'audio') {
        return TRUE; //do not allow editing of audio nodes
    }
    else {
        return FALSE;
    }
}

function hook_transcripts_editor_exclude_tiers($node)
{
    //Wylie transliteration is algorithmically derived from Tibetan script, so it should not be edited
    return array('ts_content_wylie');
}