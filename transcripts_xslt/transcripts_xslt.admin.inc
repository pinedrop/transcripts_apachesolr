<?php

function transcripts_xslt_settings_form()
{
  $form = array();

  $versions = array(
    '1.0' => t("XSLT 1.0 using PHP's libxslt extension."),
    '2.0' => t("XSLT 2.0 using Saxonica's Saxon-9"),
  );
  $form['transcripts_xslt_version'] = array(
    '#type' => 'radios',
    '#title' => t('XSLT Version'),
    '#default_value' => variable_get('transcripts_xslt_version', '2.0'),
    '#options' => $versions,
    '#description' => t("The use of XSLT 2.0 with Saxon requires that the Saxon libraries be properly installed and configured. See the module's README."),
  );

  /* XSLT 1.0 settings */
  $form['transcripts_xslt_1_settings'] = array(
    '#title' => t('XSLT 1.0 Settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t("Please enter the following additional information if you are using XSLT 1.0."),
  );
  $form['transcripts_xslt_1_settings']['transcripts_xslt_1_xml_transformer'] = array(
    '#type' => 'managed_file',
    '#title' => 'XML transformer',
    '#default_value' => variable_get('transcripts_xslt_1_xml_transformer', ''),
    '#upload_location' => 'public://transcripts_transformers',
    '#upload_validators' => array(
      'file_validate_extensions' => array('xsl'),
    ),
    '#description' => t("Upload an XSLT stylesheet that will be used to transform incoming XML transcripts into time code units."),
  );
  $desc = 'Upload an XSLT stylesheet that will be used to transform incoming TXT transcripts into time code units. '
    . 'This stylesheet should use the unparsed-text() function to load in a text file and transform it into XML. '
    . 'For example, you could use this file to convert Toolbox transcripts into XML.';
  $form['transcripts_xslt_1_settings']['transcripts_xslt_1_txt_transformer'] = array(
    '#type' => 'managed_file',
    '#title' => 'TXT transformer',
    '#default_value' => variable_get('transcripts_xslt_1_txt_transformer', ''),
    '#upload_location' => 'public://transcripts_transformers',
    '#upload_validators' => array(
      'file_validate_extensions' => array('xsl'),
    ),
    '#description' => t($desc),
  );
  $desc = 'Upload an XSLT stylesheet that will be used to transform incoming SRT transcripts into time code units. '
    . 'This stylesheet should use the unparsed-text() function to load in a SubRip file and transform it into XML.';
  $form['transcripts_xslt_1_settings']['transcripts_xslt_1_srt_transformer'] = array(
    '#type' => 'managed_file',
    '#title' => 'SRT transformer',
    '#default_value' => variable_get('transcripts_xslt_1_srt_transformer', ''),
    '#upload_location' => 'public://transcripts_transformers',
    '#upload_validators' => array(
      'file_validate_extensions' => array('xsl'),
    ),
    '#description' => t($desc),
  );
  $desc = 'Enter a comma-separated list of Toolbox tier mappings, '
    . 'each in the form TOOLBOX_TIER|TRANSCRIPTS_TIER. '
    . 'You can ignore this field if you are not importing transcripts from Toolbox.';
  $form['transcripts_xslt_1_settings']['transcripts_xslt_1_toolbox_mappings'] = array(
    '#title' => t('Toolbox mappings'),
    '#type' => 'textarea',
    '#rows' => 3,
    '#default_value' => variable_get('transcripts_xslt_1_toolbox_mappings', 'tx|ts_content_qya, mb|ts_content_morph, ge|ts_content_igt, ft|ts_content_epo'),
    '#description' => t($desc),
    '#required' => TRUE,
  );

  /* XSLT 2.0 settings - since it's the default, 2.0 is not mentioned */
  $form['transcripts_xslt_settings'] = array(
    '#title' => t('XSLT 2.0 Settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t("Please enter the following additional information if you are using XSLT 2.0."),
  );
  $form['transcripts_xslt_settings']['transcripts_xslt_saxon_jar'] = array(
    '#type' => 'textfield',
    '#title' => t('Saxon jar'),
    '#size' => 100,
    '#description' => t("Enter the name of the Saxon application jar file, specified as a relative path from the Drupal root (e.g. sites/all/libraries/saxon/saxon9he.jar)."),
    '#default_value' => variable_get('transcripts_xslt_saxon_jar', 'sites/all/libraries/saxon/saxon9he.jar'),
  );
  $form['transcripts_xslt_settings']['transcripts_xslt_xml_transformer'] = array(
    '#type' => 'managed_file',
    '#title' => 'XML transformer',
    '#default_value' => variable_get('transcripts_xslt_xml_transformer', ''),
    '#upload_location' => 'public://transcripts_transformers',
    '#upload_validators' => array(
      'file_validate_extensions' => array('xsl'),
    ),
    '#description' => t("Upload an XSLT stylesheet that will be used to transform incoming XML transcripts into time code units."),
  );
  $desc = 'Upload an XSLT stylesheet that will be used to transform incoming TXT transcripts into time code units. '
    . 'This stylesheet should use the unparsed-text() function to load in a text file and transform it into XML. '
    . 'For example, you could use this file to convert Toolbox transcripts into XML.';
  $form['transcripts_xslt_settings']['transcripts_xslt_txt_transformer'] = array(
    '#type' => 'managed_file',
    '#title' => 'TXT transformer',
    '#default_value' => variable_get('transcripts_xslt_txt_transformer', ''),
    '#upload_location' => 'public://transcripts_transformers',
    '#upload_validators' => array(
      'file_validate_extensions' => array('xsl'),
    ),
    '#description' => t($desc),
  );
  $desc = 'Upload an XSLT stylesheet that will be used to transform incoming SRT transcripts into time code units. '
    . 'This stylesheet should use the unparsed-text() function to load in a SubRip file and transform it into XML.';
  $form['transcripts_xslt_settings']['transcripts_xslt_srt_transformer'] = array(
    '#type' => 'managed_file',
    '#title' => 'SRT transformer',
    '#default_value' => variable_get('transcripts_xslt_srt_transformer', ''),
    '#upload_location' => 'public://transcripts_transformers',
    '#upload_validators' => array(
      'file_validate_extensions' => array('xsl'),
    ),
    '#description' => t($desc),
  );
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

/* http://www.michaelbagnall.com/blogs/managed-files-within-drupal-7 */
function transcripts_xslt_transformer_submit($form, $form_state)
{
  $transformers = array(
    'transcripts_xslt_xml_transformer',
    //'transcripts_xslt_txt_transformer',
    //'transcripts_xslt_srt_transformer',
    'transcripts_xslt_1_xml_transformer',
    'transcripts_xslt_1_txt_transformer',
    'transcripts_xslt_1_srt_transformer',
  );

  foreach ($transformers as $field) {
    $version = strpos($field, '1.0') === FALSE ? 'transcripts_xslt_settings' : 'transcripts_xslt_1_settings';
    $transformer = (!empty($form[$version][$field]['#default_value'])) ? $form[$version][$field]['#default_value'] : 0;
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

function transcripts_xslt_remove_transformer($transformer, $object_type)
{
  $file = $transformer ? file_load($transformer) : FALSE;
  if ($file) {
    file_usage_delete($file, 'transcripts_xslt', $object_type, $file->fid);
    file_delete($file);
    drupal_set_message(t('Transformer @transformer was removed.', array('@transformer' => $file->filename)));
  }
}

function transcripts_xslt_add_transformer($transformer, $object_type)
{
  $count = db_query('SELECT `count` FROM {file_usage} WHERE fid=:fid', array('fid' => $transformer))->fetchField();
  if (empty($count)) {
    $file = file_load($transformer);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'transcripts_xslt', $object_type, $transformer);
    drupal_set_message(t('Transformer @transformer was added.', array('@transformer' => $file->filename)));
    unset($file);
  }
}
