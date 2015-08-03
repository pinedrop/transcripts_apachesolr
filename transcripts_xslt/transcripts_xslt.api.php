<?php
/**
 * @file
 *   Exposed Hooks
 */

/**
 * Redirects from transcript/trid/tcuid to custom URL.
 *
 * @param array &$params
 *   Parameters to send to the stylesheet.
 * @param object $file
 *   The file that is being transformed.
 */
function hook_transcripts_xslt_saxon_params_alter(&$params, $file) {
    $nid = db_query('SELECT nid FROM {file_usage} WHERE fid = :fid', array(':fid' => $file->fid))->fetchField();
    $node = node_load($nid);
    $params['title'] = $node->title;
}