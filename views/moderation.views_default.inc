<?php

/**
 * Implementation of hook_views_default_views().
 */
function moderation_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'moderations';
  $view->description = 'A list of all moderations';
  $view->tag = 'moderations';
  $view->base_table = 'moderation';
  $view->human_name = 'Moderations';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Moderations';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create any moderation type';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'moderation_id' => 'moderation_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'moderation_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;

  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty ';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No moderations have been created yet';

  /* Field: Moderation: Entity_id */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'moderation';
  $handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['entity_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['entity_id']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['entity_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['entity_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['entity_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['entity_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['entity_id']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['entity_id']['format_plural'] = 0;


  /* Display: Page */
  // Automaticly generated pages for each moderation entity_type.
  $entity_infos = entity_get_info();
  $moderation_infos = moderation_get_types();

  foreach ($moderation_infos as $moderation_type => $info) {
    $handler = $view->new_display('page', 'Page', 'page_' . $info->type);

    /* Relationship: Moderation: entity_type */
    $handler->display->display_options['defaults']['relationships'] = FALSE;
    $handler->display->display_options['relationships'][$info->data['entity_type']]['id'] = $info->data['entity_type'];
    $handler->display->display_options['relationships'][$info->data['entity_type']]['table'] = 'moderation';
    $handler->display->display_options['relationships'][$info->data['entity_type']]['field'] = $info->data['entity_type'];
    $handler->display->display_options['relationships'][$info->data['entity_type']]['required'] = 0;

    /* Field: Entity: ID */
    $id = $entity_infos[$info->data['entity_type']]['entity keys']['id'];
    $handler->display->display_options['defaults']['fields'] = FALSE;
    $handler->display->display_options['fields'][$id]['id'] = $id;
    $handler->display->display_options['fields'][$id]['table'] = $entity_infos[$info->data['entity_type']]['base table'];
    $handler->display->display_options['fields'][$id]['field'] = $id;
    $handler->display->display_options['fields'][$id]['relationship'] = $info->data['entity_type'];
    $handler->display->display_options['fields'][$id]['alter']['alter_text'] = 0;
    $handler->display->display_options['fields'][$id]['alter']['make_link'] = 0;
    $handler->display->display_options['fields'][$id]['alter']['absolute'] = 0;
    $handler->display->display_options['fields'][$id]['alter']['external'] = 0;
    $handler->display->display_options['fields'][$id]['alter']['replace_spaces'] = 0;
    $handler->display->display_options['fields'][$id]['alter']['trim_whitespace'] = 0;
    $handler->display->display_options['fields'][$id]['alter']['nl2br'] = 0;
    $handler->display->display_options['fields'][$id]['alter']['word_boundary'] = 1;
    $handler->display->display_options['fields'][$id]['alter']['ellipsis'] = 1;
    $handler->display->display_options['fields'][$id]['alter']['more_link'] = 0;
    $handler->display->display_options['fields'][$id]['alter']['strip_tags'] = 0;
    $handler->display->display_options['fields'][$id]['alter']['trim'] = 0;
    $handler->display->display_options['fields'][$id]['alter']['html'] = 0;
    $handler->display->display_options['fields'][$id]['element_label_colon'] = 1;
    $handler->display->display_options['fields'][$id]['element_default_classes'] = 1;
    $handler->display->display_options['fields'][$id]['hide_empty'] = 0;
    $handler->display->display_options['fields'][$id]['empty_zero'] = 0;
    $handler->display->display_options['fields'][$id]['hide_alter_empty'] = 1;
    $handler->display->display_options['fields'][$id]['link_to_comment'] = 1;
    $handler->display->display_options['fields'][$id]['link_to_node'] = 0;
    $handler->display->display_options['path'] = 'admin/content/moderations/' . $info->type;

    /* Field: Node: Entity label */
    $handler->display->display_options['fields']['label']['id'] = 'label';
    $handler->display->display_options['fields']['label']['table'] = 'views_entity_' . $entity_infos[$info->data['entity_type']]['base table'];
    $handler->display->display_options['fields']['label']['field'] = 'label';
    $handler->display->display_options['fields']['label']['relationship'] = $info->data['entity_type'];
    $handler->display->display_options['fields']['label']['link_to_entity'] = 1;
    $handler->display->display_options['fields']['label']['ajax_preview'] = TRUE;

    /* Field: Moderation: Actions */
    $handler->display->display_options['fields']['actions']['id'] = 'actions';
    $handler->display->display_options['fields']['actions']['table'] = 'moderation';
    $handler->display->display_options['fields']['actions']['field'] = 'actions';
    $handler->display->display_options['defaults']['filter_groups'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;

    /* Filter criterion: Moderation: Entity_id */
    // Removes all entries wich have a moderation relation.
    $handler->display->display_options['defaults']['filters'] = FALSE;
    $handler->display->display_options['filters']['entity_id']['id'] = 'entity_id';
    $handler->display->display_options['filters']['entity_id']['table'] = 'moderation';
    $handler->display->display_options['filters']['entity_id']['field'] = 'entity_id';
    $handler->display->display_options['filters']['entity_id']['operator'] = 'empty';

    if (!in_array($info->data['entity_type'], unserialize(NOT_YET_SUPPORTED_ENTITIES))) {
      /* Filter criterion: Bundle: Type */
      $handler->display->display_options['filters']['type']['id'] = $entity_infos[$info->data['entity_type']]['entity keys']['bundle'];
      $handler->display->display_options['filters']['type']['table'] = 'views_entity_' . $entity_infos[$info->data['entity_type']]['base table'];
      $handler->display->display_options['filters']['type']['field'] = $entity_infos[$info->data['entity_type']]['entity keys']['bundle'];
      $handler->display->display_options['filters']['type']['relationship'] = $info->data['entity_type'];
      $handler->display->display_options['filters']['type']['value'] = array(
        $info->data['container']['bundle'] => $info->data['container']['bundle'],
      );
    }

    /* Menu tab */
    $handler->display->display_options['menu']['type'] = 'tab';
    $handler->display->display_options['menu']['title'] = $info->label;
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['menu']['name'] = 'main-menu';
    $handler->display->display_options['menu']['context'] = 0;
  }

  $views[$view->name] = $view;
  return $views;
}
