<?php

/**
 * @file
 * Moderation - Views integration
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 */

/**
 * Extend views_handler_field_comment class.
 */
class moderation_handler_field_comment_subject extends views_handler_field_comment {
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

  /**
   * Add the js / css file.
   */
  function pre_render($values) {
    parent::pre_render($values);
    drupal_add_css(drupal_get_path('module', 'moderation') . '/moderation.css');
    if (!empty($this->options['ajax_preview'])) {
      drupal_add_js(drupal_get_path('module', 'moderation') .'/moderation-views.js');
      // Just in case flags are added to the content area
      if (module_exists('flag')) {
        drupal_add_js(drupal_get_path('module', 'flag') .'/theme/flag.js');
      }
    }
  }

  function render_link($data, $values) {
    if (!empty($this->options['link_to_comment']) && $data !== NULL && $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "node/". $values->{$this->aliases['nid']};
      $this->options['alter']['fragment'] = "comment-" . $values->{$this->aliases['cid']};
      if (!empty($this->options['ajax_preview'])) {
        $this->options['alter']['link_class'] = 'moderation-ajax-preview';
      }
    }
    return $data;
  }

  function advanced_render($values) {
    $this->last_render = parent::advanced_render($values);
    if (!empty($this->options['ajax_preview'])) {
      $this->last_render .= '<div type="comment" entity-id="' . $values->{$this->aliases['cid']} . '" class="moderation-preview-attributes"></div>';
    }
    return $this->last_render;
  }
}