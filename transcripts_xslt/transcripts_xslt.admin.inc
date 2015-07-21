<?php

function transcripts_xslt_settings_form()
{
    $form = array();

    $form['transcripts_xslt_settings'] = array(
        '#title' => t('XSLT settings'),
        '#type' => 'fieldset',
        '#collapsed' => FALSE,
    );
    $form['transcripts_xslt_settings']['transcripts_xslt_saxon_path'] = array(
        '#type' => 'textfield',
        '#title' => t('Saxon directory'),
        '#size' => 80,
        '#maxlength' => 100,
        '#description' => t("The relative path from the Drupal root to the Saxon directory. All library jars must be in the same directory. Do not include a leading slash."),
        '#default_value' => variable_get('transcripts_xslt_saxon_path', 'sites/all/libraries/saxon'),
    );
    $form['transcripts_xslt_settings']['transcripts_xslt_saxon_jar'] = array(
        '#type' => 'textfield',
        '#title' => t('Saxon jar'),
        '#size' => 20,
        '#description' => t("The name of the Saxon application jar file (e.g. saxon9he.jar)."),
        '#default_value' => variable_get('transcripts_xslt_saxon_jar', 'saxon9he.jar'),
    );
    $form['transcripts_xslt_settings']['transcripts_xslt_xml_transformer'] = array(
        '#type' => 'managed_file',
        '#title' => 'XML transformer',
        '#default_value' => variable_get('transcripts_xml_transformer', ''),
        '#upload_location' => 'public://transcript-transformers',
        '#upload_validators' => array(
            'file_validate_extensions' => array('xsl'),
        ),
        '#description' => t("Upload an XSLT stylesheet that will be used to transform incoming XML transcripts into time code units."),
    );
    $form['transcripts_xslt_settings']['transcripts_xslt_txt_transformer'] = array(
        '#type' => 'managed_file',
        '#title' => 'TXT transformer',
        '#default_value' => variable_get('transcripts_txt_transformer', ''),
        '#upload_location' => 'public://transcript-transformers',
        '#upload_validators' => array(
            'file_validate_extensions' => array('xsl'),
        ),
        '#description' => t("Upload an XSLT stylesheet that will be used to transform incoming TXT transcripts into time code units."),
    );
    /*
    $form['transcripts_xslt_settings']['transcripts_xslt_default_transformer'] = array(
        '#type' => 'textfield',
        '#title' => t('Default XSLT'),
        '#size' => 20,
        '#description' => t("The default XSLT file to be used to transform incoming transcripts into a format to be used by Drupal and Solr."),
        '#default_value' => variable_get('transcripts_xslt_default_transformer', 'transcripts.xsl'),
    );
    $message = 'On occasion, you may wish to use custom XSLT files to transform incoming transcripts into time code units. '
        . 'If this is the case, then specify a file field here. '
        . 'The field should be attached to one or more of the "permissible node types" above. '
        . 'You can then upload alternative stylesheets to the field on a node by node basis.';
    $form['transcripts_xslt_settings']['transcripts_xslt_transformer_field'] = array(
        '#type' => 'textfield',
        '#title' => t('XSLT overrides'),
        '#size' => 20,
        '#description' => t($message),
        '#default_value' => variable_get('transcripts_xslt_transformer_field', 'field_transcripts_xsl'),
    );*/
    $desc = 'Enter a comma-separated list of Toolbox tier mappings, '
        . 'each in the form TOOLBOX_TIER|TRANSCRIPTS_TIER. '
        . 'You can ignore this field if you are not importing transcripts from Toolbox.';
    $form['transcripts_xslt_settings']['transcripts_xslt_toolbox_mappings'] = array(
        '#title' => t('Toolbox mappings'),
        '#type' => 'textarea',
        '#rows' => 3,
        '#default_value' => variable_get('transcripts_xslt_toolbox_mappings', 'tx|ts_content_qya, mb|ts_content_morph, ge|ts_content_igt, ft|ts_content_epo'),
        '#description' => t($desc),
        '#required' => TRUE,
    );

    $form = system_settings_form($form);
    $form['#submit'][] = 'transcripts_xslt_transformer_submit';

    return $form;
}

function transcripts_xslt_transformer_submit($form, $form_state)
{
    $transformers = array('transcripts_xslt_xml_transformer', 'transcripts_xslt_txt_transformer');

    foreach ($transformers as $field) {
        $transformer = (!empty($form[$field]['#default_value'])) ? $form[$field]['#default_value'] : 0;
        if (isset($form_state['values'][$field]) && $form_state['values'][$field]) {
            if (isset($transformer) && $transformer != $form_state['values'][$field]) {
                transcripts_xslt_remove_transformer($transformer, 'admin_setting');
                transcripts_xslt_add_transformer($form_state['values'][$field], 'admin_setting');
            } else {
                transcripts_xslt_add_transformer($form_state['values'][$field], 'admin_setting');
            }
        } elseif ($transformer) {
            transcripts_xslt_remove_transformer($transformer, 'admin_setting');
        }
    }
}

function transcripts_xslt_remove_transformer($transformer, $object_type) {
    $file = $transformer ? file_load($transformer) : FALSE;
    if ($file) {
        file_usage_delete($file, 'transcripts_xslt', $object_type, $file->fid);
        file_delete($file);
        drupal_set_message(t('The transformer @transformer was removed.', array('@transformer' => $file->filename)));
    }
}

function transcripts_xslt_add_transformer($transformer, $object_type) {
    $count = db_query('SELECT `count` FROM {file_usage} WHERE fid=:fid', array('fid' => $transformer))->fetchField();
    if (empty($count)) {
        $file = file_load($transformer);
        $file->status = FILE_STATUS_PERMANENT;
        file_save($file);
        file_usage_add($file, 'transcripts_xslt', $object_type, $transformer);
        unset($file);
    }
}