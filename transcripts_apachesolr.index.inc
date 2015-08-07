<?php

function transcripts_apachesolr_already_indexed($module, $type, $id, $fid = NULL)
{
    $query = db_select('transcripts_apachesolr_transcript', 'tr')
        ->fields('tr', array('trid'))
        ->condition('tr.module', $module, '=')
        ->condition('tr.type', $type, '=')
        ->condition('tr.id', $id, '=');
    if ($fid !== NULL) {
        $query->condition('tr.fid', $fid, '=');
    }
    $result = $query->execute();

    return $result->rowCount() == 0 ? FALSE : $result;
}

function transcripts_apachesolr_already_indexed_trid($module, $type, $id, $fid = NULL)
{
    $result = transcripts_apachesolr_already_indexed($module, $type, $id, $fid);
    if ($result) {
        $arr = $result->fetchAssoc();
        return $arr['trid'];
    } else {
        return FALSE;
    }
}

function transcripts_apachesolr_insert_id($module, $type, $id, $fid) {
    db_update('transcripts_apachesolr_transcript')
        ->fields(array('id' => $id))
        ->condition('module', $module)
        ->condition('type', $type)
        ->condition('fid', $fid)
        ->execute();
}

function transcripts_apachesolr_remove_transcript($module, $type, $id, $fid = NULL)
{
    if ($fid == NULL) {
        $trids = db_select('transcripts_apachesolr_transcript', 'tr')
            ->fields('tr', array('trid'))
            ->condition('module', $module, '=')
            ->condition('type', $type, '=')
            ->condition('id', $id, '=')
            ->execute()
            ->fetchCol();
    }
    else {
        $trids = db_select('transcripts_apachesolr_transcript', 'tr')
            ->fields('tr', array('trid'))
            ->condition('module', $module, '=')
            ->condition('type', $type, '=')
            ->condition('id', $id, '=')
            ->condition('fid', $fid, '=')
            ->execute()
            ->fetchCol();
    }

    if (count($trids) > 0) {
        $tcuids = db_select('tcu', 't')
            ->fields('t', array('tcuid'))
            ->condition('trid', $trids, 'IN')
            ->execute()
            ->fetchCol();

        if (count($tcuids) > 0) {
            tcu_delete_multiple($tcuids);
        }

        db_delete('transcripts_apachesolr_transcript')
            ->condition('trid', $trids, 'IN')
            ->execute();
    }
}

function transcripts_apachesolr_add_transcript($module, $type, $fid, $id, $tcus)
{
    //check that tcus have been created
    if (isset($tcus['tcu'])) {
        $trid = db_insert('transcripts_apachesolr_transcript')
            ->fields(array(
                'module' => $module,
                'type' => $type,
                'fid' => $fid,
                'id' => $id,
                'status' => 1,
            ))
            ->execute();

        //fix for http://tapadoo.com/2011/regular-problem-with-xml-to-json-converters/
        if (isset($tcus['tcu'][0])) { //numeric array, more than one tcu
            foreach ($tcus['tcu'] as $tcu) {
                transcripts_apachesolr_insert_tcu($trid, $tcu);
            }
        } else {
            transcripts_apachesolr_insert_tcu($trid, $tcus['tcu']); //non-numeric single tcu
        }
    }
}

function transcripts_apachesolr_insert_tcu($trid, $tcu)
{
    $insert = tcu_create(
        array(
            'trid' => $trid,
            'start' => isset($tcu['start']) ? $tcu['start'] : 0,
            'end' => isset($tcu['end']) ? $tcu['end'] : 0,
            'created' => REQUEST_TIME,
            'changed' => REQUEST_TIME,
        )
    );
    if (isset($tcu['speakers'])) {
        $insert->setSpeakers(array_filter(array_map('trim', $tcu['speakers']))); //don't save empty speaker tiers
    }
    if (isset($tcu['tiers'])) {
        $insert->setTiers(array_filter(array_map('trim', $tcu['tiers']))); //don't save empty tiers
    }
    tcu_save($insert);
}

function transcripts_apachesolr_queue_transcripts($nids = '')
{
    $fields = array_filter(transcripts_node_all_file_fields(), 'is_transcript_field');

    $query = db_select('transcripts_apachesolr_transcript', 'tr')
        ->fields('tr')
        ->condition('tr.status', 1, '=');

    if ($nids) {
        $query->condition('tr.id', explode('+', $nids), 'IN');
    }
    else {
        $query->condition('tr.id', 0, '!=');
    }
    $result = $query->execute();

    while ($record = $result->fetchAssoc()) {
        $node = node_load($record['id']);

        foreach ($fields as $field_name) {
            if (isset($node->$field_name)) {
                $field = $node->$field_name;

                if (!(empty($field) || (empty($field[$node->language]) && empty($field['und'])))) {
                    $lang = empty($field[$node->language]) ? 'und' : $node->language;

                    $file = file_load($field[$lang][0]['fid']);

                    $reindex = TRUE;
                    foreach (module_implements('transcripts_apachesolr_queue_exclude') as $module) {
                        $exclude = module_invoke($module, 'transcripts_apachesolr_queue_exclude', $file, $node);
                        // If the hook returns TRUE we should exclude the node
                        if (!empty($exclude)) {
                            $reindex = FALSE;
                        }
                    }

                    if ($reindex) {
                        db_update('transcripts_apachesolr_transcript')
                            ->fields(array('status' => 0))
                            ->condition('module', 'transcripts_node', '=')
                            ->condition('type', 'node', '=')
                            ->condition('id', $node->nid, '=')
                            ->execute();
                    }
                }
            }
        }
    }
}
