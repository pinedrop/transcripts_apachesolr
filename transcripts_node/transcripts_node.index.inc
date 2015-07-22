<?php

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

            if (!(empty($field) || empty($field['und']))) {
                $fid = $field['und'][0]['fid'];
                transcripts_node_add_transcript(file_load($fid), node_load($node->nid));
            }
        }
    }
}

function transcripts_node_node_update($node)
{
    $fields = array_filter(transcripts_node_all_file_fields(), 'is_transcript_field');

    foreach ($fields as $field_name) {
        if (isset($node->$field_name)) {
            $field = $node->$field_name;

            if (empty($field) || empty($field['und'])) {
                transcripts_apachesolr_remove_transcript('transcripts_node', 'node', $node->nid);
            } else {
                $fid = $field['und'][0]['fid'];
                if (!transcripts_apachesolr_already_indexed('transcripts_node', 'node', $node->nid, $fid)) {
                    transcripts_apachesolr_remove_transcript('transcripts_node', 'node', $node->nid);
                    transcripts_node_add_transcript(file_load($fid), node_load($node->nid));
                }
            }
        }
    }
}

function transcripts_node_add_transcript($file, $node)
{
    $tcus = transcripts_xslt_as_tcus($file, $node);
    if ($tcus !== false) {
        transcripts_apachesolr_add_transcript('transcripts_node', 'node', $file->fid, $node->nid, $tcus);
    }
}
