<?php

function transcripts_node_node_settings()
{
    $form = array();

    $form['transcripts_node_node_settings'] = array(
        '#title' => t('Node settings'),
        '#type' => 'fieldset',
        '#collapsed' => FALSE,
    );
    $default = implode(' ', transcripts_node_default_transcript_extensions());
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

function transcripts_node_videotag_settings()
{
    /* interpret id - should permissions check for PHP writing ability */
    $form = array();

    $form['transcripts_node_videotag_settings'] = array(
        '#title' => t('Video tag'),
        '#type' => 'fieldset',
        '#collapsed' => FALSE,
    );
    $form['transcripts_node_videotag_settings']['transcripts_node_id_mapper'] = array(
        '#title' => t('Id mapper'),
        '#description' => t('<p>PHP code that interprets unique (node) ids as passed to URLs, and converts them to data used by the
							transcript players. Do not begin this code with <code>&lt;?php</code>. Code should end with:
							<div>
							    <code>
								    $val = array(<br/>
									&nbsp;&nbsp;"video_tag" => "HTML 5 VIDEO TAG",<br/>
									&nbsp;&nbsp;"video_url" => array(URL1, URL2, ...),<br/>
									);<br/>
								</code>
							</div>
  						</p>
  						<p>
  						  The id to be mapped by this code is contained in the variable <code>$entity_id</code>.
  						  If <code>video_url</code> is returned, then a video tag will be constructed from
  						  the supplied array. Otherwise, <code>video_tag</code> will be used.
  						</p>'),
        '#type' => 'textarea',
        '#rows' => 10,
        '#default_value' => variable_get('transcripts_node_id_mapper', ''),
    );

    return $form;
}

function transcripts_node_site_settings_form($form, &$form_state)
{
    return system_settings_form(array_merge(
        $form,
        transcripts_node_node_settings(),
        transcripts_node_videotag_settings()
    ));
}
