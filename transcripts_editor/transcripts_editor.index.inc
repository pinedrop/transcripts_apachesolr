<?php

/**
 * Define default flags.
 */
function transcripts_editor_flag_default_flags()
{
    $info = system_get_info('module', 'flag');
    $api_version = substr($info['version'], 4, 1); //7.x-2 or 7.x-3

    if ($api_version == 2 || $api_version == 3) {
        //supported versions
        $flags = array();
        $flags['keep_transcript_edits'] = array(
            'content_type' => 'node',
            'title' => 'Keep transcript edits',
            'global' => 1,
            'types' => array(
                0 => 'audio',
                1 => 'video',
            ),
            'flag_short' => 'Keep transcript edits',
            'flag_long' => 'Deselect to revert to the uploaded transcript.',
            'flag_message' => 'Transcript edits will be kept.',
            'unflag_short' => 'Discard transcript edits',
            'unflag_long' => 'Discard transcript edits, and revert to the uploaded transcript',
            'unflag_message' => 'Transcript edits have been discarded, and will revert to the uploaded transcript.',
            'unflag_denied_text' => '',
            'link_type' => 'toggle',
            'roles' => array(
                'flag' => array(
                    0 => 2,
                ),
                'unflag' => array(
                    0 => 2,
                ),
            ),
            'weight' => 0,
            'show_on_form' => 1,
            'access_author' => '',
            'show_on_page' => 0,
            'show_on_teaser' => 0,
            'show_contextual_link' => 0,
            'i18n' => 0,
            'module' => 'transcripts_editor',
            'api_version' => $api_version,
        );
        return $flags;
    } else {
        //unsupported versions
        return array();
    }
}

function transcripts_editor_flag_access($flag, $entity_id, $action, $account)
{
    if ($flag->name == 'keep_transcript_edits') {
        if ($action == 'flag') {
            //this flag is only intended for programmatic use
            return FALSE;
        } else if ($action == 'unflag') {
            //only allow those who can edit a node to unflag it
            return (node_access('update', node_load($entity_id))) ? TRUE : FALSE;
        }
    }
}

/**
 * Implements hook_flag (Flag 2.x)
 *
 * @param $action
 * @param $flag
 * @param $content_id
 * @param $account
 */
function transcripts_editor_flag($action, $flag, $content_id, $account)
{
    if ($flag->name == 'keep_transcript_edits') {
        if ($action == 'unflag') transcripts_editor_discard_edits($content_id);
    }
}

/**
 * Implements hook_flag_unflag (Flag 3.x)
 *
 * @param $flag
 * @param $entity_id
 * @param $account
 * @param $flagging
 */
function transcripts_editor_flag_unflag($flag, $entity_id, $account, $flagging)
{
    if ($flag->name == 'keep_transcript_edits') {
        transcripts_editor_discard_edits($entity_id);
    }
}

function transcripts_editor_discard_edits($entity_id)
{
    $transcript = db_select('transcripts_apachesolr_transcript', 't')
        ->fields('t')
        ->condition('module', 'transcripts_node', '=')
        ->condition('type', 'node', '=')
        ->condition('id', $entity_id, '=')
        ->execute()
        ->fetchObject();

    transcripts_apachesolr_remove_transcript('transcripts_node', 'node', $entity_id);
    transcripts_node_add_transcript(file_load($transcript->fid), $entity_id);
}

function transcripts_editor_transcripts_apachesolr_queue_exclude($file, $node)
{
    $flag = flag_get_flag('keep_transcript_edits');
    if ($flag && $flag->is_flagged($node->nid)) {
        return TRUE;
    } else {
        return FALSE;
    }
}