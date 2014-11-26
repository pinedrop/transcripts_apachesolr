<?php

function transcripts_xslt_settings_form() {
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
        );
	
	return system_settings_form($form);
}
