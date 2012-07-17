<?php

/**
 * @file
 * Contains the moderation_handler_filter_entity_bundle class.
 */

/**
 * Generic filter class to fiter entities by bundle.
 */
class moderation_handler_filter_entity_bundle extends views_handler_filter_in_operator {

  /**
   * Stores the entity type on which the filter filters.
   *
   * @var string
   */
  public $entity_type;

  /**
   * Override the init method to set the proper table.
   */
  function init(&$view, &$options) {
    parent::init($view, $options);

    $data = views_fetch_data($this->table);
    $this->entity_type = $data['table']['entity type'];
    $entity_info = entity_get_info($this->entity_type);
    $this->table = $entity_info['base table'];
  }


  function query() {
    $this->ensure_my_table();

    // Adjust the join for the comment case.
    if ($this->entity_type == 'comment') {
      $join = new views_join();
      $def = array(
        'table' => 'node',
        'field' => 'nid',
        'left_table' => $this->relationship,
        'left_field' => 'nid',
      );
      $join->definition = $def;
      $join->construct();
      $join->adjusted = TRUE;
      $this->table_alias = $this->query->add_table('node', $this->relationship, $join);
      $this->real_field = 'type';

      // Replace the value to match the node type column.
      foreach ($this->value as &$value) {
        $value = str_replace('comment_node_', '', $value);
      }
    }

    parent::query();
  }


  /**
   * Get all bundles for this entity type.
   *
   * @see field_info_bundles().
   *
   * @return array
   *   A list of bundles for this entity type.
   */
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Content types');

      if ($this->entity_type == 'node') {
        $types = node_type_get_types();
      }
      else {
        $types = field_info_bundles($this->entity_type);
        foreach ($types as $type_id => $type) {
          $type['name'] = $type['label'];
          $types[$type_id] = (object) $type;
        }
      }

      $options = array();
      foreach ($types as $type => $info) {
        $options[$type] = t($info->name);
      }
      asort($options);
      $this->value_options = $options;
    }
  }

}

