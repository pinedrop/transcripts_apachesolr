<?php

function transcripts_apachesolr_admin()
{
    return drupal_get_form('transcripts_apachesolr_configuration_form');
}

function transcripts_apachesolr_configuration_form($form, &$form_state)
{
    $form = array();
    $form['action'] = array(
        '#type' => 'fieldset',
        '#title' => t('Actions'),
        '#collapsible' => TRUE,
    );
    $form['action']['queue_nids'] = array(
        '#type' => 'textarea',
        '#description' => t('If you only wish to queue some transcripts, enter their node ids here, separated by the plus sign. For example: 185, or 185+240.'),
    );
    $form['action']['queue'] = array(
        '#prefix' => '<div>',
        '#suffix' => '</div>',
        '#type' => 'submit',
        '#value' => t('Queue transcripts'),
        '#submit' => array('transcripts_apachesolr_index_transcripts_form_queue_submit'),
    );
    $form['action']['index'] = array(
        '#prefix' => '<div>',
        '#type' => 'submit',
        '#value' => t('Index queued transcripts'),
        '#submit' => array('transcripts_apachesolr_index_transcripts_form_index_submit'),
    );
    $form['action']['index_description'] = array(
        '#prefix' => '<span>',
        '#suffix' => '</span></div>',
        '#markup' => t('This could take time and could put an increased load on your server.'),
    );

    $environments = apachesolr_load_all_environments();
    foreach ($environments as $id => $environment) {
        $options[$id] = $environment['name'];
    }
    $form['transcripts_apachesolr_env_id'] = array(
        '#title' => t('Search environment'),
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => transcripts_apachesolr_environment(),
        '#description' => t('Select the Apache Solr search environment that this module will use to index time code units.'),
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save configuration'),
    );

    return $form;
}

function transcripts_apachesolr_configuration_form_submit($form, &$form_state)
{
    if (isset($form_state['values']['transcripts_apachesolr_env_id'])) {
        variable_set('transcripts_apachesolr_env_id', $form_state['values']['transcripts_apachesolr_env_id']);
        drupal_set_message(t('Your configuration has been saved.'));
    }
}

function transcripts_apachesolr_index_transcripts_form_queue_submit(array $form, array &$form_state)
{
    $redirect = 'admin/config/user-interface/transcripts/apachesolr/queue';
    if (isset($form_state['input']['queue_nids'])) {
        $redirect .= "/" . check_plain($form_state['input']['queue_nids']);
    }

    $destination = array();
    if (isset($_GET['destination'])) {
        $destination = drupal_get_destination();
        unset($_GET['destination']);
    }
    $form_state['redirect'] = array($redirect, array('query' => $destination));
}


function transcripts_apachesolr_index_transcripts_form_queue_confirm(array $form, array &$form_state, $nids = '')
{
    $message = $nids == '' ? t('All transcripts will be reindexed. ') : t('Transcripts from these nodes will be reindexed: ') . str_replace(' ', '+', trim($nids)) . '. ';

    return confirm_form($form,
        t('Are you sure you want to queue transcripts for reindexing?'),
        'admin/config/user-interface/transcripts/apachesolr',
        $message . t('Any changes in the transcript index will be overwritten.'),
        t('Queue transcripts')
    );
}

function transcripts_apachesolr_index_transcripts_form_queue_confirm_submit(array $form, array &$form_state)
{
    if (empty($form_state['build_info']['args'])) { //queue all transcripts
        transcripts_apachesolr_queue_transcripts();
    } else { //queue selected transcripts
        transcripts_apachesolr_queue_transcripts(str_replace(' ', '+', trim($form_state['build_info']['args'][0])));
    }
    $form_state['redirect'] = 'admin/config/user-interface/transcripts/apachesolr';
    drupal_set_message(t('Transcripts queued for re-indexing. You must now index these transcripts.'));
}

function transcripts_apachesolr_index_transcripts_form_index_submit(array $form, array &$form_state)
{
    $redirect = 'admin/config/user-interface/transcripts/apachesolr/index';

    $destination = array();
    if (isset($_GET['destination'])) {
        $destination = drupal_get_destination();
        unset($_GET['destination']);
    }
    $form_state['redirect'] = array($redirect, array('query' => $destination));
}

function transcripts_apachesolr_index_transcripts_form_index_confirm(array $form, array &$form_state)
{
    return confirm_form($form,
        t('Are you sure you want index the queued transcripts?'),
        'admin/config/user-interface/transcripts/apachesolr',
        NULL,
        t('Index transcripts')
    );
}

function transcripts_apachesolr_index_transcripts_form_index_confirm_submit(array $form, array &$form_state)
{
    $form_state['redirect'] = 'admin/config/user-interface/transcripts/apachesolr';

    $result = db_select('transcripts_apachesolr_transcript', 'tr')
        ->fields('tr')
        ->condition('tr.id', 0, '!=')
        ->condition('tr.status', 0, '=')
        ->execute();

    $operations = array();
    while ($record = $result->fetchAssoc()) {
        $node = node_load($record['id']);
        $operations[] = array('transcripts_apachesolr_index_one_transcript', array($node));
    }

    batch_set(array(
        'operations' => $operations,
        'finished' => 'transcripts_apachesolr_index_queued_transcripts_finished',
    ));
}

function transcripts_apachesolr_index_one_transcript($node, &$context)
{
    $context['results'][] = $node->nid . ' : ' . check_plain($node->title);
    $context['message'] = t('[node/@nid] Indexing transcript...', array('@nid' => $node->nid));

    $fields = array_filter(transcripts_node_all_file_fields(), 'is_transcript_field');
    foreach ($fields as $field_name) {
        if (isset($node->$field_name)) {
            $field = $node->$field_name;

            if (!(empty($field) || (empty($field[$node->language]) && empty($field['und'])))) {
                $lang = empty($field[$node->language]) ? 'und' : $node->language;

                $file = file_load($field[$lang][0]['fid']);
                transcripts_apachesolr_remove_transcript('transcripts_node', 'node', $node->nid);
                if (transcripts_node_add_transcript($file, $node->nid)) {
                    drupal_set_message("[node/{$node->nid}] Indexed {$file->filename}");
                    db_update('transcripts_apachesolr_transcript')
                        ->fields(array('status' => 1))
                        ->condition('module', 'transcripts_node', '=')
                        ->condition('type', 'node', '=')
                        ->condition('id', $node->nid, '=')
                        ->execute();
                } else {
                    drupal_set_message("[node/{$node->nid}] FAILED to index {$file->filename}", 'error');
                }
            }
        }
    }
}

function transcripts_apachesolr_index_queued_transcripts_finished($success, $results, $operations)
{
    if ($success) {
        drupal_set_message(t('Indexed @count transcripts.', array('@count' => count($results))));
    } else {
        $error_operation = reset($operations);
        drupal_set_message(t('An error occurred while processing @operation with arguments : @args', array('@operation' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE))));
    }
}