<?php

/**
 * @file
 * Contains the moderation_handler_filter_entity_bundle class.
 */

/**
 * Generic filter class to fiter entities by bundle.
 */
class moderation_handler_filter_entity_bundle extends views_handler_filter_in_operator {

  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Content types');
      if ($this->options['relationship'] != 'none') {
        $entity_type = $this->options['relationship'];
        $this->table = $this->options['relationship'];
      }
      else {
        $entity_type = $this->view->base_table;
        $this->table = $this->view->base_table;
      }

      if ($entity_type == 'node') {
        $types = node_type_get_types();
      }
      else {
        $types = field_info_bundles($entity_type);
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

