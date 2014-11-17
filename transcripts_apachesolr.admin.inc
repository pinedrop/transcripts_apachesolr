<?php

function transcripts_apachesolr_admin() {
        $environments = apachesolr_load_all_environments();
        foreach ($environments as $id => $environment) {
                $options[$id] = $environment['name'];
        }
	$form = array();

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
