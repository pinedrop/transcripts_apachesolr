<?php

function transcripts_apachesolr_drush_help($command) {
    switch ($command) {
        case 'drush:transcripts-solr-index':
            return dt('(Re-)index queued transcripts.');
        case 'drush:transcripts-solr-queue':
            return dt('Queue transcripts for (re-)indexing.');
    }
}

function transcripts_apachesolr_drush_command() {
    $items = array();
    $items['transcripts-solr-index'] = array(
        'description' => dt('(Re-)index queued transcripts.'),
        'examples' => array(
            'Basic usage' => 'drush transcripts-solr-index',
        ),
        'aliases' => array('tsi'),
    );
    $items['transcripts-solr-queue'] = array(
        'description' => dt('Queue transcripts for (re-)indexing.'),
        'arguments' => array(
            'nids' => dt('Optional list of nodes to queue, separated by the plus symbol.'),
        ),
        'examples' => array(
            'Queue all nodes' => 'drush transcripts-solr-queue',
            'Queue selected nodes' => 'drush transcripts-solr-queue 5+1234+87'
        ),
        'aliases' => array('tsq'),
    );
    return $items;
}

function drush_transcripts_apachesolr_transcripts_solr_index() {
    $fields = array_filter(transcripts_node_all_file_fields(), 'is_transcript_field');

    $result = db_select('transcripts_apachesolr_transcript', 't')
        ->fields('t')
        ->condition('status', 0, '=')
        ->execute();

    while ($record = $result->fetchAssoc()) {
        $node = node_load($record['id']);

        transcripts_apachesolr_remove_transcript('transcripts_node', 'node', $node->nid);
        foreach ($fields as $field_name) {
            if (isset($node->$field_name)) {
                $field = $node->$field_name;

                if (!(empty($field) || (empty($field[$node->language]) && empty($field['und'])))) {
                    $lang = empty($field[$node->language]) ? 'und' : $node->language;

                    $file = file_load($field[$lang][0]['fid']);
                    transcripts_apachesolr_remove_transcript('transcripts_node', 'node', $node->nid);
                    if (transcripts_node_add_transcript($file, $node->nid)) {
                        drush_log("[node/{$node->nid}] Indexed {$file->filename}.", 'ok');
                    }
                    else {
                        drush_log("[node/{$node->nid}] FAILED to index {$file->filename}.", 'error');
                    }
                }
            }
        }
    }
}