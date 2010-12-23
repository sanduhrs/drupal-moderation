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
      $this->additional_fields['sticky'] = array('table' => 'node', 'field' => 'sticky');
      $this->additional_fields['promote'] = array('table' => 'node', 'field' => 'promote');
      $this->additional_fields['status'] = array('table' => 'node', 'field' => 'status');
    }
    elseif ($base_table == 'comments') {
      $this->additional_fields['id'] = array('table' => 'comments', 'field' => 'cid');
      $this->additional_fields['status'] = array('table' => 'comments', 'field' => 'status');
//       $this->additional_fields['moderation'] = array('table' => 'status', 'field' => 'nid');
    }
  }
  function render($values) {
    $base_table = $this->table;

    $entity = new stdClass();
    $entity->id = $values->{$this->aliases['id']};
    $entity->status = $values->{$this->aliases['status']};
    if ($base_table == 'node') {
      $entity_type = 'node';
      $entity->sticky = $values->{$this->aliases['sticky']};
      $entity->promote = $values->{$this->aliases['promote']};
    }
    elseif ($base_table == 'comments') {
      $entity_type = 'comment';
    }

    $token = drupal_get_token($entity_type . '-'. $entity->id);
    $moderation = moderation_get_last($entity_type, $entity->id);
    return theme('moderation_item_links', $entity, $entity_type, $moderation, $token);
  }
}