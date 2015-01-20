<?php
/**
 * @file
 *   Exposed Hooks
 */

/**
 * Redirects from transcript/trid/tcuid to custom URL.
 *
 * @param string $transcript
 *   DB row from table transcripts_apachesolr_transcript.
 * @param string $fragment
 *   Tcu fragment identifier, in the form tcu/tcuid.
 */
function hook_transcripts_apachesolr_redirect($transcript, $fragment = NULL)
{
    /* redirect to node/nid/transcript#tcu/tcuid */

    $path = 'node/' . $transcript['id'] . '/transcript';
    if ($fragment === NULL) {
        drupal_goto($path);
    } else {
        drupal_goto($path, array('fragment' => $fragment));
    }
}
