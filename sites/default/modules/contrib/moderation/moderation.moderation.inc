<?php
// $Id: moderation.moderation.inc,v 1.1.2.11 2010/07/09 07:53:07 sanduhrs Exp $

/**
 * @file
 * Moderation - nodes and comments queue
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 */

/**
 * Mixed Node / Comment queue; Menu callback
 */
function moderation_queue() {
  drupal_add_css(drupal_get_path('module', 'moderation') .'/moderation.css');
  $settings = array('moderationType' => 'thread');
  drupal_add_js('jQuery.extend(Drupal.settings, '. drupal_to_js($settings) .')', 'inline');
  drupal_add_js(drupal_get_path('module', 'moderation') .'/moderation.js');

  if (module_exists('flag')) {
    drupal_add_js(drupal_get_path('module', 'flag') .'/theme/flag.js');
  }

  $query = "SELECT DISTINCT(n.nid) FROM {node} n
              INNER JOIN {comments} c ON n.nid = c.nid
              INNER JOIN {moderation_moderation} mm ON c.cid = mm.obj_id
                WHERE mm.obj_type = 'comment'
                  AND (mm.status IS NULL OR mm.status=0)
                  AND n.type IN ('" . implode('\' ,\'', variable_get('moderation_thread_types', array())) ."')
                ORDER BY n.created DESC, n.title ASC";
  $count_query = "SELECT COUNT(DISTINCT(n.nid)) FROM {node} n
              INNER JOIN {comments} c ON n.nid = c.nid
              INNER JOIN {moderation_moderation} mm ON c.cid = mm.obj_id
                WHERE mm.obj_type = 'comment'
                  AND (mm.status IS NULL OR mm.status=0)
                  AND n.type IN ('" . implode('\' ,\'', variable_get('moderation_thread_types', array())) ."')";
  $result = pager_query(db_rewrite_sql($query), 50, 0, $count_query);

  $destination = drupal_get_destination();

  while ($row = db_fetch_object($result)) {
    $node = node_load(array('nid' => $row->nid));
    $moderation = moderation_get_last('node', $node->nid);
    $token = drupal_get_token('node-'. $node->nid);

    $item = theme('moderation_item_thread', $node, $moderation, $token);

    $rows[] = array('data' =>
      array(
        array(
          'data' => $item,
        ),
      )
    );
  }

  if (!$rows) {
    $output = t('No threads available.');
  }

  $output .= '<div class="moderation-queue moderation-queue-thread">' . theme('table', array(), $rows) . '</div>';
  $output .= theme('pager', NULL, 50);

  return $output;
}

/**
 * Mixed Node / Comment queue; Menu callback
 */
function moderation_flag_queue() {
  drupal_add_css(drupal_get_path('module', 'moderation') .'/moderation.css');
  $settings = array('moderationType' => 'flag');
  drupal_add_js('jQuery.extend(Drupal.settings, '. drupal_to_js($settings) .')', 'inline');
  drupal_add_js(drupal_get_path('module', 'moderation') .'/moderation.js');

  if (module_exists('flag')) {
    drupal_add_js(drupal_get_path('module', 'flag') .'/theme/flag.js');
  }

  $flag_id = variable_get('moderation_thread_flag', '');
  $query = "SELECT DISTINCT(n.nid) FROM {node} n
              INNER JOIN {comments} c ON n.nid = c.nid
              INNER JOIN {flag_content} fc ON fc.content_id = c.cid
                WHERE fc.content_type = 'comment'
                  AND fc.fid = $flag_id
                ORDER BY n.created DESC, n.title ASC";
  $count_query = "SELECT COUNT(DISTINCT(n.nid)) FROM {node} n
              INNER JOIN {comments} c ON n.nid = c.nid
              INNER JOIN {flag_content} fc ON fc.content_id = c.cid
                WHERE fc.content_type = 'comment'
                  AND fc.fid = $flag_id
                ORDER BY n.created DESC, n.title ASC";
  $result = pager_query(db_rewrite_sql($query), 50, 0, $count_query);

  $destination = drupal_get_destination();

  while ($row = db_fetch_object($result)) {
    $node = node_load(array('nid' => $row->nid));
    $moderation = moderation_get_last('node', $node->nid);
    $token = drupal_get_token('node-'. $node->nid);

    $item = theme('moderation_item_flag', $node, $moderation, $token);

    $rows[] = array('data' =>
      array(
        array(
          'data' => $item,
        ),
      )
    );
  }

  if (!$rows) {
    $output = t('No flagged threads available.');
  }

  $output .= '<div class="moderation-queue moderation-queue-flag">' . theme('table', array(), $rows) . '</div>';
  $output .= theme('pager', NULL, 50);

  return $output;
}

/*
 * Menu callback: content administration.
 */
function moderation_node_queue() {
  drupal_add_css(drupal_get_path('module', 'moderation') .'/moderation.css');
  $settings = array('moderationType' => 'node');
  drupal_add_js('jQuery.extend(Drupal.settings, '. drupal_to_js($settings) .')', 'inline');
  drupal_add_js(drupal_get_path('module', 'moderation') .'/moderation.js');

  $query = "SELECT n.nid FROM {node} n
              INNER JOIN {moderation_moderation} mm ON n.nid = mm.obj_id
                WHERE mm.obj_type = 'node'
                  AND (mm.status IS NULL OR mm.status=0)
                  AND n.type IN ('" . implode('\' ,\'', variable_get('moderation_node_types', array())) ."')
                ORDER BY n.created DESC, n.title ASC";
  $result = pager_query(db_rewrite_sql($query), 50);

  $destination = drupal_get_destination();

  while ($row = db_fetch_object($result)) {
    $node = node_load(array('nid' => $row->nid));
    $moderation = moderation_get_last('node', $node->nid);
    $token = drupal_get_token('node-'. $node->nid);

    $item = theme('moderation_item_node', $node, $moderation, $token);

    $rows[] = array('data' =>
      array(
        array(
          'data' => $item,
        ),
      )
    );
  }

  if (!$rows) {
    $output = t('No posts available.');
  }

  $output .= '<div class="moderation-queue moderation-queue-node">' . theme('table', array(), $rows) . '</div>';
  $output .= theme('pager', NULL, 50);

  return $output;
}

/**
 * Menu callback; present an administrative comment listing.
 */
function moderation_comment_queue() {
  drupal_add_css(drupal_get_path('module', 'moderation') .'/moderation.css');
  $settings = array('moderationType' => 'comment');
  drupal_add_js('jQuery.extend(Drupal.settings, '. drupal_to_js($settings) .')', 'inline');
  drupal_add_js(drupal_get_path('module', 'moderation') .'/moderation.js');

  if (module_exists('flag')) {
    drupal_add_js(drupal_get_path('module', 'flag') .'/theme/flag.js');
  }

  $query = "SELECT c.cid FROM {comments} c
           LEFT JOIN {moderation_moderation} mm ON c.cid = mm.obj_id
           WHERE mm.obj_type = 'comment'
             AND (mm.status IS NULL OR mm.status=0)
           ORDER BY c.timestamp DESC, c.subject ASC";
  $result = pager_query(db_rewrite_sql($query), 50);
  $destination = drupal_get_destination();

  while ($row = db_fetch_object($result)) {
    $comment = _comment_load($row->cid);
    $moderation = moderation_get_last('comment', $comment->cid);
    $token = drupal_get_token('comment-'. $comment->cid);
    $node = node_load(array('nid' => $comment->nid));

    $item = theme('moderation_item_comment', $comment, $moderation, $node, $token);

    $rows[] = array('data' =>
      array(
        array(
          'data' => $item,
        ),
      )
    );
  }

  if (!$rows) {
    $output = t('No comments available.');
  }

  $output .= '<div class="moderation-queue moderation-queue-comment">' . theme('table', array(), $rows) . '</div>';
  $output .= theme('pager', NULL, 50);
  return $output;
}

function moderation_log($type = 'node', $obj_id = NULL) {

  switch ($type) {
    case 'comment':
      $sql = "SELECT m.*, c.nid, c.cid, c.subject, u.uid, u.name FROM {moderation} m
                LEFT JOIN {comments} c ON c.cid = m.obj_id
                LEFT JOIN {users} u ON u.uid = m.uid
                WHERE m.obj_type = 'comment' ". ($obj_id ? 'AND m.obj_id = '. $obj_id : '') ." ORDER BY m.created DESC";
      break;
    case 'node':
      $sql = "SELECT m.*, n.title, n.nid, u.uid, u.name FROM {moderation} m
                LEFT JOIN {node} n ON n.nid = m.obj_id
                LEFT JOIN {users} u ON u.uid = m.uid
                WHERE m.obj_type = 'node' ". ($obj_id ? 'AND m.obj_id = '. $obj_id : '') ." ORDER BY m.created DESC";
      break;
  }

  $result = pager_query($sql, 25, 0, NULL);
  while ($moderation = db_fetch_object($result)) {
    $title = $moderation->subject ? $moderation->subject : $moderation->title;
    $fragment = $moderation->cid ? 'comment-'. $moderation->cid : NULL;

    $user->uid = $moderation->uid;
    $user->name = $moderation->name;

    $moderations[] = array(
      l($title, 'node/'. $moderation->nid, array('fragment' => $fragment)),
      theme('username', $user),
      moderation_log_message($moderation->attribute, $moderation->status, $type),
      format_date($moderation->created),
    );
  }

  $header = array(
    t('Title'), t('User'), t('Action'), t('Date'),
  );
  $output = theme('table', $header, $moderations);
  $output .= theme('pager');
  return $output;
}
