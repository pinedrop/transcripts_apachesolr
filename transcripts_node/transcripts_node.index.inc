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

function transcripts_node_node_update($node)
{
    $fields = array_filter(transcripts_node_all_file_fields(), 'is_transcript_field');

    foreach ($fields as $field_name) {
        if (isset($node->$field_name)) {
            $field = $node->$field_name;

            if (empty($field) || (empty($field[$node->language]) && empty($field['und']))) {
                $orig = $node->original->$field_name;
                if (!(empty($orig) || (empty($orig[$node->language]) && empty($orig['und'])))) {
                    transcripts_apachesolr_remove_transcript('transcripts_node', 'node', $node->nid);
                    drupal_set_message("Your transcript has been removed.", 'status');
                }
            } else {
                $lang = empty($field[$node->language]) ? 'und' : $node->language;

                $file = file_load($field[$lang][0]['fid']);
                if (!transcripts_apachesolr_already_indexed('transcripts_node', 'node', $node->nid, $file->fid)) {
                    transcripts_apachesolr_remove_transcript('transcripts_node', 'node', $node->nid);

                    if (transcripts_node_add_transcript($file, node_load($node->nid))) {
                        watchdog('transcripts_node', 'Transcript at node/' .$node->nid. ' was removed, replaced, and indexed.', NULL, WATCHDOG_DEBUG);
                    } else {
                        watchdog('transcripts_node', 'Transcript at node/' .$node->nid. ' was removed and replaced, but could not be indexed.', NULL, WATCHDOG_DEBUG);
                    }
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
