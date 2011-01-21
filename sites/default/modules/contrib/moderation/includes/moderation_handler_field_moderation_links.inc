<?php
// $Id$

class moderation_handler_field_moderation_links extends views_handler_field {
  function query() {
    $this->add_additional_fields();
  }
  /**
   * Constructor to provide additional field to add.
   */
  function init(&$view, $options) {
    parent::init($view, $options);
    $base_table = $this->table;
    // Add different fields dependening on the entity_type.
    if ($base_table == 'node') {
      $this->additional_fields['id'] = array('table' => 'node', 'field' => 'nid');
      $this->additional_fields['promote'] = array('table' => 'node', 'field' => 'promote');
      $this->additional_fields['moderate'] = array('table' => 'node', 'field' => 'moderate');
      $this->additional_fields['sticky'] = array('table' => 'node', 'field' => 'sticky');
    }
    elseif ($base_table == 'comments') {
      $this->additional_fields['id'] = array('table' => 'comments', 'field' => 'cid');
      $this->additional_fields['status'] = array('table' => 'comments', 'field' => 'status');
      $this->additional_fields['moderate'] = array('table' => 'moderation_comment_status', 'field' => 'status');
    }
  }

  function render($values) {
    $base_table = $this->table;
    $indent_prefix = $indent_suffix = '';

    $entity = new stdClass();
    $entity->id = $values->{$this->aliases['id']};
    if ($base_table == 'node') {
      $entity_type = 'node';
      $entity->promote = $values->{$this->aliases['promote']};
      $entity->moderate = $values->{$this->aliases['moderate']};
      $entity->sticky = $values->{$this->aliases['sticky']};
    }
    elseif ($base_table == 'comments') {
      $entity_type = 'comment';
      $entity->status = $values->{$this->aliases['status']};
      $entity->moderate = $values->{$this->aliases['moderate']};
    }

    $token = drupal_get_token($entity_type . '-'. $entity->id);
    return theme('moderation_item_links', $entity, $entity_type, $entity->moderate, $token);
  }
}
