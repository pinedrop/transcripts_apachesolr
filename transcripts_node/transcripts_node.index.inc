<?php

function transcripts_node_file_delete($file) {
    //remove any orphaned transcript associated with this file
    transcripts_apachesolr_remove_transcript('transcripts_node', 'node', 0, $file->fid);
}

function transcripts_node_node_delete($node)
{
    $fields = array_filter(transcripts_node_all_file_fields(), 'is_transcript_field');

    if (count($fields) > 0) {
        transcripts_apachesolr_remove_transcript('transcripts_node', 'node', $node->nid);
    }
}

function transcripts_node_node_insert($node)
{
    $fields = array_filter(transcripts_node_all_file_fields(), 'is_transcript_field');

    foreach ($fields as $field_name) {
        if (isset($node->$field_name)) {
            $field = $node->$field_name;

            if (!(empty($field) || (empty($field[$node->language]) && empty($field['und'])))) {
                $lang = empty($field[$node->language]) ? 'und' : $node->language;

                $file = file_load($field[$lang][0]['fid']);
                if (transcripts_apachesolr_already_indexed('transcripts_node', 'node', 0, $file->fid)) {
                    transcripts_apachesolr_insert_id('transcripts_node', 'node', $node->nid, $file->fid);
                }
            }
        }
    }
}

function transcripts_node_add_transcript($file, $nid)
{
    $tcus = transcripts_xslt_as_tcus($file);
    if ($tcus === false) {
        return false;
    } else {
        transcripts_apachesolr_add_transcript('transcripts_node', 'node', $file->fid, $nid, $tcus);
        return true;
    }
}
