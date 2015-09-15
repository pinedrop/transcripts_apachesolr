<?php

function hook_transcripts_editor_exclude_tiers($node)
{
    //Wylie transliteration is algorithmically derived from Tibetan script, so it should not be edited
    return array('ts_content_wylie');
}