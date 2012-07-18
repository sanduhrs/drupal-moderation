<?php

/**
 * UI controller.
 */
class ModerationTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage moderation entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }

}

/**
 * Generates the moderation type editing form.
 */
function moderation_type_form($form, &$form_state, $moderation_type, $op = 'edit') {

  if ($op == 'clone') {
    $moderation_type->label .= ' (cloned)';
    $moderation_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $moderation_type->label,
    '#description' => t('The human-readable name of this moderation type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($moderation_type->type) ? $moderation_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'moderation_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this moderation type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $entites = array();
  foreach (field_info_bundles() as $entity => $entity_bundles) {
    // Filter the moderation entity.
    if ($entity != 'moderation' && $entity != 'moderation_type') {
      $entites[$entity] = $entity;
      foreach ($entity_bundles as $bundle_key => $bundle) {
        $bundles[$entity][$bundle_key] = $bundle['label'];
      }
    }
  }

  $rule_configs = entity_load('rules_config');
  $moderation_rule_configs = array();
  foreach ($rule_configs as $rule_config) {
    if (in_array('moderation', $rule_config->dependencies)) {
      $moderation_rule_configs[$rule_config->name] = $rule_config;
    }
  }

  $form['data']['#tree'] = TRUE;

  if (isset($form_state['values']['data']['entity_type'])) {
    $active_entity_type = $form_state['values']['data']['entity_type'];
  }
  else {
    $active_entity_type = isset($moderation_type->data['entity_type']) ? $moderation_type->data['entity_type'] : reset(array_keys($entites));
  }

  $form['data']['entity_type'] = array(
    '#title' => t('Entity type'),
    '#type' => 'select',
    '#description' => t('The entity type to be moderated.'),
    '#required' => TRUE,
    '#default_value' => $active_entity_type,
    '#options' => $entites,
    '#ajax' => array(
      'callback' => 'moderation_type_container_callback',
      'wrapper' => 'moderation-type-container',
      'method' => 'replace',
    ),
  );

  $actions = moderation_action_list($active_entity_type);
  $entity_actions = array();
  foreach ($actions as $action_id => $action) {
    $entity_actions[$action_id] = $label = isset($action['label callback']) ? call_user_func($action['label callback'], $action, $active_entity_type) : moderation_action_label($action, $active_entity_type);
  }

  $form['data']['container'] = array(
    '#type' => 'container',
    '#attributes' => array('id' => 'moderation-type-container'),
  );

  $disabled = FALSE;
  // Disable the bundle selection for not supported entities to not let the user
  // expect to be abel to use them in their queue.
  if(in_array($active_entity_type, unserialize(NOT_YET_SUPPORTED_ENTITIES))) {
    $disabled = TRUE;
  }
  $form['data']['container']['bundle'] = array(
    '#title' => t('Bundle'),
    '#type' => 'select',
    '#default_value' => isset($moderation_type->data['container']['bundle']) ? $moderation_type->data['container']['bundle'] : reset(array_keys($bundles[$active_entity_type])),
    '#description' => t('The bundle type to be moderated.'),
    '#required' => TRUE,
    '#options' => $bundles[$active_entity_type],
    '#disabled' => $disabled,
  );
  $form['data']['container']['actions'] = array(
    '#title' => t('Actions'),
    '#type' => 'checkboxes',
    '#default_value' => isset($moderation_type->data['container']['actions']) ? $moderation_type->data['container']['actions'] : array(),
    '#description' => t('The bundle type to be moderated.'),
    '#required' => TRUE,
    '#options' => $entity_actions,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save moderation type'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Ajax callback to return the bundle container.
 */
function moderation_type_container_callback($form, &$form_state) {
  return $form['data']['container'];
}

/**
 * Form API submit callback for the type form.
 */
function moderation_type_form_submit(&$form, &$form_state) {
  $moderation_type = entity_ui_form_submit_build_entity($form, $form_state);
  $moderation_type->save();
  // Redirect directly to the moderation queue.
  $form_state['redirect'] = 'admin/moderations/' . $moderation_type->type;
}

/**
 * Form API submit callback for the delete button.
 */
function moderation_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/moderation_types/manage/' . $form_state['moderation_type']->type . '/delete';
}
