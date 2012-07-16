<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying the entity specific
 * actions.
 */
class moderation_handler_actions_field extends views_handler_field {

  function construct() {
    parent::construct();

    $this->additional_fields['moderation_id'] = 'moderation_id';
    $this->additional_fields['type'] = 'type';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    drupal_add_library('system', 'drupal.ajax');
    $output = '';
    // Not the nicest way.
    $moderation_type = preg_replace('/^page_/', '', $this->view->current_display);
    $moderation = moderation_type_load($moderation_type);
    $moderation_type_actions = $moderation->data['container']['actions'];
    $moderation_actions = moderation_action_list();
    $entity_type = $moderation->data['entity_type'];

    $entity_info = entity_get_info($entity_type);
    $entity_type_id_key = $entity_info['entity keys']['id'];

    $base_table = $entity_info['base table'];

    $entity_id_key = $base_table . '_moderation_' . $entity_type_id_key;
    if (isset($entity_id_key)) {
      $entity_id = $values->$entity_id_key;
    }

    $callbacks = array();
    $labels = array();
//
//    foreach ($moderation_type_actions as $moderation_type_action_name => $value) {
//      if (!is_numeric($value)) {
//        $label = $moderation_actions[$moderation->data['entity_type']][$moderation_type_action_name]['label'];
//        $callbacks[$moderation_type_action_name] = $moderation_actions[$moderation->data['entity_type']][$moderation_type_action_name]['callback'];
//        $callback = $moderation_actions[$moderation->data['entity_type']][$moderation_type_action_name]['callback'];
//        // Callback page with entity arguments.
//        $output .= '<div id="myDiv_' . $entity_id . '"></div><div>' . l($label, 'moderation_ajax_actions_callback/nojs/' . $callback . '/' . $entity_type . '/' . $entity_id, array('attributes' => array('class' => array('use-ajax')))) . '</div>';
//        $output .= ' ';
//      }
//    }

    $output = '';
    $moderation_type = preg_replace('/^page_/', '', $this->view->current_display);
    $moderation = moderation_type_load($moderation_type);

    foreach (moderation_action_list($entity_type) as $callback => $action) {
      if (isset($action['event']) && (isset($moderation->data['container']['actions'][$callback]) && !empty($moderation->data['container']['actions'][$callback]))) {

        $label = isset($action['label callback']) ? call_user_func($action['label callback'], $action, $entity_type, $entity_id) : moderation_action_label($action, $entity_type, $entity_id);

        $output .= '<div id="moderation_action_' . $entity_id . '_' . $action['event'] . '">' . l($label, 'moderation_ajax_actions_callback/nojs/' . $action['event'] . '/' . $entity_type . '/' . $entity_id, array('attributes' => array('class' => array('use-ajax')))) . '</div>';
      }
    }

    return $output;
  }

}
