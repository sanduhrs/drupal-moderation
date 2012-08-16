<?php

/**
 * Extend entity_views_handler_field_label. See http://drupal.org/node/1435418.
 */
class moderation_handler_entity_field_label extends entity_views_handler_field_label {

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
  function render($values) {
    $attributes = array();
    if (($entity = $this->get_value($values)) && entity_access('view', $this->entity_type, $entity)) {
      $entity_info = entity_get_info($this->entity_type);
      $label = entity_label($this->entity_type, $entity);
      $entity_uri = entity_uri($this->entity_type, $entity);
      $path = $entity_uri['path'];
      
      if (!empty($this->options['ajax_preview'])) {
        drupal_add_library('system', 'drupal.ajax');
        $attributes = array('attributes' => array('class' => array('use-ajax')));
        // Pretty ugly.
        $moderation_type = arg(2);
        $path = 'moderation_entity_view/nojs/' . $this->entity_type . '/' . $entity->{$entity_info['entity keys']['id']} . '/' . $moderation_type;
      }

      $data = '<div>' . l($label, $path, $attributes) . '</div>';
      $data .= '<div id="title_' . $entity->{$entity_info['entity keys']['id']} . '"></div>';

      return $data;
    }
  }

}
