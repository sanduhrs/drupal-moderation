<?php

/**
 * Implementation of hook_rules_event_info().
 */
function moderation_rules_event_info() {
  foreach (moderation_action_list() as $entity_type => $action) {
    foreach ($action as $key => $value) {
      if (isset($value['event'])) {
        $events[$value['event']] = array(
          'label' => $value['event'],
          'group' => t('Moderation'),
          'variables' => array(
            'entity' => array(
              'type' => 'entity',
              'label' => t('Entity'),
            ),
          ),
        );
      }
    }
  }
  $events['moderation_on_moderate'] = array(
    'label' => t('On moderate'),
    'group' => t('Moderation'),
    'variables' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t('Entity'),
      ),
    ),
  );
  return $events;
}

/**
 * Implementation of hook_rules_action_info().
 */
function moderation_rules_action_info() {
  return array(
    'moderations_action_moderate' => array(
      'label' => t('Moderate'),
      'group' => t('Moderation'),
      'parameter' => array(
        'entity' => array(
          'type' => 'entity',
          'label' => t('Entity'),
        ),
      ),
      'module' => 'moderation',
    ),
  );
}

/**
 * Callback function for moderation_rules_action_info().
 * This function will create an moderation entity, holding the relation of the
 * moderated entity.
 *
 * @param type $entity
 */
function moderations_action_moderate($entity) {
  // Fetch entity out of the entity_metadata_wrapper.
  $entity_type = $entity->type();
  $entity = $entity->value();
  $entity_info = entity_get_info($entity_type);
  $entity_id = $entity->{$entity_info['entity keys']['id']};

  // Create moderation entity with the relation we need.
  $moderation = new Moderation();
  $moderation->entity_id = $entity_id;
  $moderation->entity_type = $entity_type;

  $moderation->save();
}
