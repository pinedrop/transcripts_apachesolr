<?php

function transcripts_node_node_settings()
{
    $form = array();

    $form['transcripts_node_node_settings'] = array(
        '#title' => t('Node settings'),
        '#type' => 'fieldset',
        '#collapsed' => FALSE,
    );
    $default = implode(' ', transcripts_node_default_file_extensions());
    $form['transcripts_node_node_settings']['transcripts_node_file_extensions'] = array(
        '#type' => 'textfield',
        '#title' => t('Permissible file extensions'),
        '#default_value' => variable_get('transcripts_node_file_extensions', $default),
        '#size' => 80,
        '#maxlength' => 255,
        '#description' => t('List all permissible transcript file extensions, separated by whitespace and without the leading dot.'),
    );
    foreach (entity_get_info() as $entity_type => $entity_info) {
        if ($entity_type == 'node' && !empty($entity_info['apachesolr']['indexable'])) {
            $options = array();
            foreach ($entity_info['bundles'] as $key => $info) {
                $options[$key] = $info['label'];
            }
            $form['transcripts_node_node_settings']['entities']['transcripts_node_' . $entity_type] = array(
                '#type' => 'checkboxes',
                '#title' => t('Permissible node types'),
                '#options' => $options,
                '#default_value' => variable_get('transcripts_node_' . $entity_type, array()),
                '#description' => t('Select the node types that are allowed to have transcripts.'),
            );
        }
    }

    return $form;
}

function transcripts_node_site_settings_form($form, &$form_state)
{
    return system_settings_form(array_merge(
        $form,
        transcripts_node_node_settings()
    ));
}
