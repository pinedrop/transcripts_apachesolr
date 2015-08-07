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
    $form['action']['queue'] = array(
        '#prefix' => '<div>',
        '#suffix' => '</div>',
        '#type' => 'submit',
        '#value' => t('Queue all transcripts for reindexing'),
        '#submit' => array('transcripts_apachesolr_index_transcripts_form_queue_submit'),
    );
    /*$form['action']['index'] = array(
        '#prefix' => '<div>',
        '#type' => 'submit',
        '#value' => t('Re-index all queued content'),
        '#submit' => array('transcripts_apachesolr_index_transcripts_form_index_submit'),
    );
    $form['action']['index_description'] = array(
        '#prefix' => '<span>',
        '#suffix' => '</span></div>',
        '#markup' => t('Could take time and could put an increased load on your server.'),
    );*/

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

function transcripts_apachesolr_configuration_form_submit(&$form_state)
{
    if (isset($form_state['values']['transcripts_apachesolr_env_id'])) {
        variable_set('transcripts_apachesolr_env_id', $form_state['values']['transcripts_apachesolr_env_id']);
        drupal_set_message(t('Your configuration has been saved.'));
    }
}

function transcripts_apachesolr_index_transcripts_form_queue_submit(array $form, array &$form_state)
{
    $destination = array();
    if (isset($_GET['destination'])) {
        $destination = drupal_get_destination();
        unset($_GET['destination']);
    }
    $form_state['redirect'] = array('admin/config/user-interface/transcripts/apachesolr/queue', array('query' => $destination));
}


function transcripts_apachesolr_index_transcripts_form_queue_confirm(array $form, array &$form_state)
{
    return confirm_form($form,
        t('Are you sure you want to queue transcripts for reindexing?'),
        'admin/config/user-interface/transcripts/apachesolr',
        t('Uploaded transcripts will be used to regenerate the transcript index. Any changes in the transcript index will be overwritten.'),
        t('Queue transcripts')
    );
}

function transcripts_apachesolr_index_transcripts_form_queue_confirm_submit(array $form, array &$form_state)
{
    $form_state['redirect'] = 'admin/config/user-interface/transcripts/apachesolr';
    transcripts_apachesolr_queue_transcripts();
    drupal_set_message(t('Transcripts queued for re-indexing. You must now index these transcripts.'));
}
