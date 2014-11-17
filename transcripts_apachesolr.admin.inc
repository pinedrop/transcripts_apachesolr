<?php

function transcripts_apachesolr_settings_form($form, &$form_state) {
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
                '#weight' => -30,
        );

        return system_settings_form($form);
}

function transcripts_ui_site_settings_form($form, &$form_state) {
	return system_settings_form(array_merge(
		$form,
		transcripts_ui_solr_settings(),
		transcripts_ui_style_settings(),
		transcripts_ui_tier_settings(),
		transcripts_ui_profile_settings()
	));
}

