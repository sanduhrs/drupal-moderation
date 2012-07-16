<?php

function moderation_rules_event_info() {
  return array(
    'moderation_on_moderate' => array(
      'label' => t('On moderate'),
      'group' => t('Moderation'),
      'variables' => array(
        'entity' => array(
          'type' => 'entity',
          'label' => t('Entity'),
        ),
      ),
    ),
  );
}

/**
 * implementation of hook_rules_condition_info()
 */
function moderation_rules_condition_info() {
  return array(
    'moderation_is_of_type' => array(
      'label' => t('Moderation is of type'),
      'group' => t('Moderation'),
      'parameter' => array(
        'entity' => array(
          'type' => 'entity',
          'label' => t('Entity'),
          'description' => t('Specifies the entity for which to evaluate the condition.'),
        ),
        'type' => array(
          'type' => 'token',
          'label' => t('Entity type'),
          'description' => t('The entity type to check for.'),
          'options list' => 'rules_entity_action_type_options',
          'restriction' => 'input',
        ),
      ),
    ),
  );
}

/**
 * Assert the selected entity type.
 */
function moderation_is_of_type($entity, $entity_type) {
  $entity_info = entity_get_info($entity_type);
  $bundle_key = $entity_info['entity keys']['bundle'];

  if ($type = $entity->$bundle_key) {
    return array('entity' => array('type' => $type));
  }
}
