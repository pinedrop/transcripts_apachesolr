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
    $result = db_select('transcripts_apachesolr_transcript', 't')
        ->fields('t')
        ->condition('status', 0, '=')
        ->execute();

    while ($record = $result->fetchAssoc()) {
        print_r($record);
    }
}