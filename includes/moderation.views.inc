<?php
// $Id$

/**
 * @file
 * Moderation - Views integration
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 */

/**
 * Implements hook_views_data_alter().
 */
function moderation_views_data_alter(&$data) {
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

  $data['comments']['subject']['field']['handler'] = 'moderation_handler_field_comment_subject';
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
  $data['moderation_comment_status']['table']['group']  = t('Moderation');
  $data['moderation_comment_status']['table']['join'] = array(
    'comments' => array(
      'left_field' => 'cid',
      'field' => 'cid',
      'type' => 'left',
    ),
    'node' => array(
      'left_table' => 'comments',
      'left_field' => 'cid',
      'field' => 'cid',
    ),
  );

  // Comment moderated or not
  // Nodes already have their own implementation
  $data['moderation_comment_status']['status'] = array(
    'title' => t('Is moderated'),
    'help' => t('Whether or not the comment has been moderated.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Moderated'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
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
