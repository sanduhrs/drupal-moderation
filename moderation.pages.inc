<?php
// $Id$

/**
 * @file
 * Moderation - nodes and comments queue
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 */

/**
 * Menu callback;
 */
function moderation_callback_switch($obj_type, $obj_id, $op, $attribute) {
  $js = isset($_REQUEST['js']);
  $token = $_REQUEST['token'];

  // Check for valid token
  // We don't need a token if $op is get
  if (!drupal_valid_token($token, $obj_type .'-'. $obj_id) && $op != 'get') {
    drupal_access_denied();
    exit();
  }

  $attributes = array('status', 'promote', 'sticky', 'moderate', 'preview');
  $types      = array('node', 'comment', 'thread', 'flag');

  if (!is_numeric($obj_id) OR !in_array($obj_type, $types) OR !in_array($attribute, $attributes)) {
    return drupal_not_found();
  }

  if ($op == 'get') {
    if ($attribute == 'preview') {
      if ($js) {
        // Return content for preview if js is available
        moderation_get_preview($obj_id, $obj_type);
      }
      else {
        // Redirect to node/comment if js is not available
        moderation_goto($obj_id, $obj_type);
      }
    }
    else {
      // Return the current value for an attribute
      moderation_get_attribute($obj_id, $obj_type, $attribute);
    }
    exit();
  }

  if ($op == 'set') {
    switch ($attribute) {
      case 'status':
      case 'promote':
      case 'sticky':
        $result = moderation_switch_attribute($obj_id, $obj_type, $attribute);
        break;
      case 'moderate':
        $result = moderation_switch_moderation($obj_id, $obj_type);
        break;
    }

    if ($js) {
      $GLOBALS['devel_shutdown'] = FALSE;
      print moderation_json_encode($result);
      exit();
    }
    else {
      drupal_set_message(t('The changes have been saved.'));
      drupal_goto();
    }
  }

  return drupal_not_found();
}

/**
 * Redirect to a node/comment
 */
function moderation_goto($obj_id, $obj_type) {
  if ($obj_type == 'node') {
    drupal_goto('node/'. $obj_id);
  }
  else {
    if ($comment = _comment_load($obj_id)) {
      drupal_goto('node/'. $comment->nid, NULL, 'comment-'. $comment->cid);
    }
  }
}

/**
 * Switch moderation flag
 *
 * @param string $obj_type one of 'node', 'comment'
 * @param integer $obj_id
 */
function moderation_switch_moderation($obj_id, $obj_type) {
  global $user;
  $status = moderation_get_status($obj_type, $obj_id);
  moderation_insert_moderation_log($obj_type, $obj_id, !$status, 'moderate', $user);
  $result = moderation_update_moderation($obj_type, $obj_id, !$status);
  return array($result, !$status, 'moderate', $obj_id, $obj_type);
}

/**
 * Switch an attribute
 *
 * @param integer $obj_id
 * @param string $obj_type one of 'node', 'comment'
 * @param string $attribute one of 'status', 'promote', 'sticky'
 */
function moderation_switch_attribute($obj_id, $obj_type, $attribute) {
  global $user;
  if ($obj_type == 'node') {
    $status_new = !db_result(db_query("SELECT %s FROM {node} WHERE nid=%d", $attribute, $obj_id));
    $success = db_query("UPDATE {node} SET %s=%d WHERE nid=%d", $attribute, $status_new, $obj_id);
  }
  elseif ($obj_type == 'comment') {
    $status_new = !db_result(db_query("SELECT %s FROM {comments} WHERE cid=%d", $attribute, $obj_id));
    $success = db_query("UPDATE {comments} SET %s=%d WHERE cid=%d", $attribute, $status_new, $obj_id);
  }
  $result = moderation_insert_moderation_log($obj_type, $obj_id, !$status, $attribute, $user);
  return array($result, $status_new, $attribute, $obj_type);
}

/**
 * Get the preview markup for a node or a comment
 *
 * @param integer $obj_id
 * @param string $obj_type
 */
function moderation_get_preview($obj_id, $obj_type) {
  $data = '';
  switch ($obj_type) {
    case 'node':
      if ($node = node_load($obj_id)) {
        $data = theme('moderation_node_preview', $node);
      }
      break;
    case 'comment':
      if ($comment = _comment_load($obj_id)) {
        $_GET['q'] = 'node/'. $comment->nid;
        $node = node_load($comment->nid);
        $data = '<div class="preview">'. theme('comment_view', $comment, $node) .'</div>';
      }
      break;
    case 'thread':
      if ($node = node_load($obj_id)) {

        // fix drupal_get_destination issue for propper redirection
        $_REQUEST['destination'] = 'admin/content/moderation';

        $data = theme('moderation_node_preview', $node);
        $data .= theme('moderation_comments_preview', $node);
      }
      break;
  }
  print moderation_json_encode($data);
  exit();
}

/**
 * Get an objects attribute
 *
 * @param integer $obj_id
 * @param string $obj_type
 * @param string $attribute one of 'status', 'promote', 'sticky', 'moderate'
 */
function moderation_get_attribute($obj_id, $obj_type, $attribute) {
  $table = ($obj_type == 'comment') ? 'comments' : 'node';
  $id    = ($obj_type == 'comment') ? 'cid'      : 'nid';

  moderation_get_status($obj_type, $obj_id);
  if ($attribute == 'moderate') {
    $status = moderation_get_status($obj_type, $obj_id);
    print moderation_json_encode(array($status, $obj_id, $obj_type));
  }
  else {
    $status = db_result(db_query("SELECT %s FROM {%s} WHERE %s=%d", $attribute , $table, $id, $obj_id));
    print moderation_json_encode(array($status, $obj_id, $obj_type));
  }
  exit();
}

/**
 * Log pages; Menu callback
 */
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
    $title      = $moderation->subject ? $moderation->subject : $moderation->title;
    $fragment   = $moderation->cid ? 'comment-'. $moderation->cid : NULL;

    $account = new stdClass();
    $account->uid  = $moderation->uid;
    $account->name = $moderation->name;

    $moderations[] = array(
      l($title, 'node/'. $moderation->nid, array('fragment' => $fragment)),
      theme('username', $account),
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
