<?php

/**
 * @file
 * Providing extra functionality for the Moderation UI via views.
 */

/**
 * Implementation of hook_views_data()
 */
function moderation_views_data_alter(&$data) {
  $data['moderation']['actions'] = array(
    'field' => array(
      'title' => t('Actions'),
      'help' => t('Provide the action Links of this moderation.'),
      'handler' => 'moderation_handler_actions_field',
    ),
  );

  $entity_infos = entity_get_info();
  foreach ($entity_infos as $entity_type => $info) {

    $data['views_entity_' . $info['base table']]['label']['field']['handler'] = 'moderation_handler_entity_field_label';

    if ($entity_type != 'moderation' && $entity_type != 'moderation_type') {
      // Relationship from moderation to all entity types.
      // A not relation to use with filter moderation.entity_id = NULL.
      $data['moderation'][$entity_type] = array(
        'title' => t('@type', array('@type' => $entity_type)),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'type' => 'RIGHT',
          'label' => check_plain($entity_type),
          'base' => $info['base table'],
          'base field' => $info['entity keys']['id'],
          'relationship field' => 'entity_id',
          // @todo: check, this might be "extras".
          'extra' => array(
            array(
              'table' => 'moderation',
              'field' => 'entity_type',
              'operator' => '=',
              'value' => $entity_type,
            ),
          ),
        ),
      );
    }

    // Expose generally usable entity-related fields.
    foreach (entity_get_info() as $entity_type => $info) {
      if (isset($info['entity keys']['bundle'])) {
        $data['views_entity_' . $entity_type][$info['entity keys']['bundle']] = array(
          'title' => t('Bundle'),
          'help' => t('The bundle type (for example, "basic page", "node comment", "story", etc).'),
          'filter' => array(
            'handler' => 'moderation_handler_filter_entity_bundle',
          ),
        );
      }
    }
  }
}
