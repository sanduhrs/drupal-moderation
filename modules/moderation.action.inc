<?php

/**
 * Implementation of hook_moderation_action_info().
 */
function moderation_moderation_action_info() {
  $moderation_actions = array();
  $moderation_actions['every_entity']['moderation_moderate_entity'] = array(
    'event' => 'moderation_on_moderate',
    'label callback' => 'moderation_moderate_action_label',
    'label data' => array(
      'data' => 'moderate',
      'labels' => array(
        0 => t('moderate'),
        1 => t('unmoderate'),
      ),
    ),
  );

  return $moderation_actions;
}

/**
 * Label callback for moderation action link.
 */
function moderation_moderate_action_label($action, $entity_type, $entity_id = null) {
  if (isset($entity_id)) {
    $result = db_select('moderation', 'm')
            ->fields('m', array('moderation_id'))
            ->condition('entity_type', $entity_type, '=')
            ->condition('entity_id', $entity_id, '=')
            ->execute()
            ->fetchAssoc();
    if (isset($result['moderation_id'])) {
      return t('unmoderate');
    }
    else {
      return t('moderate');
    }
  }
  else {
    return t('moderate') . ' | ' . t('unmoderate');
  }
}
