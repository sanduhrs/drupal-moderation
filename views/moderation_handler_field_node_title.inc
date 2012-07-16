<?php
// TODO: make this work with all entities.
/**
 * Extend views_handler_field_node class
 */
class moderation_handler_field_node_title extends views_handler_field_node {

  function option_definition() {
    $options = parent::option_definition();
    $options['ajax_preview'] = array('bool' => TRUE, 'default' => FALSE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['ajax_preview'] = array(
      '#type' => 'checkbox',
      '#title' => t('Ajax preview'),
      '#description' => t('Should a click on the link automatically show a preview?'),
      '#default_value' => $this->options['ajax_preview'],
      '#dependency' => '',
    );
  }

  // TODO: multilingual support.
  function render_link($data, $values) {
    if (!empty($this->options['ajax_preview'])) {
      drupal_add_library('system', 'drupal.ajax');
      $entity_type = 'node';
      $entity_id = $values->{$this->aliases['nid']};
      $label = $values->node_moderation_title;

      $data = '<div>' . l($label, 'moderation_entity_view/nojs/' . $entity_type . '/' . $entity_id, array('attributes' => array('class' => array('use-ajax')))) . '</div>';
      $data .= '<div id="title_' . $entity_id . '"></div>';
    }
    return $data;
  }

}
