<?php
// $Id$

/**
 * Implements hook_views_data_alter().
 */
function moderation_views_data_alter(&$data) {
  $data['comments']['subject']['field']['handler'] = 'moderation_handler_field_comment_subject';

  $data['node']['title']['field']['handler'] = 'moderation_handler_field_node_title';
  $data['node']['title']['field']['moderation type'] = 'node';

  $data['node']['moderation_links'] = array(
    'title' => t('Moderation links (Node)'),
    'help' => t('Moderation links (Node)'),
    'group' => t('Moderation'),
    'field' => array(
      'handler' => 'moderation_handler_field_moderation_links',
    ),
  );
  $data['node']['thread_title'] = array(
    'title' => t('Thread title'),
    'group' => t('Moderation'),
    'real field' => 'title',
    'field' => array(
      'moderation type' => 'thread',
      'handler' => 'moderation_handler_field_node_title',
    ),
  );

  $data['comments']['moderation_links'] = array(
    'title' => t('Moderation links (Comment)'),
    'help' => t('Moderation links (Comment)'),
    'group' => t('Moderation'),
    'field' => array(
      'handler' => 'moderation_handler_field_moderation_links',
    ),
  );
}

/**
 * Implements hook_views_data().
 */
function moderation_views_data() {
  $data['moderation']['table']['group']  = t('Moderation');
  $data['moderation']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'obj_id',
    'extra' => array(
      'field' => 'obj_type',
      'operator' => '=',
      'value' => 'node',
    ),
  );
  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function moderation_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'moderation') . '/includes',
    ),
    'handlers' => array(
      // Field handler.
      'moderation_handler_field_comment_subject' => array(
        'parent' => 'views_handler_field_comment'
      ),
      'moderation_handler_field_node_title' => array(
        'parent' => 'views_handler_field_node',
      ),
      'moderation_handler_field_moderation_links' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
