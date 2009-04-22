<?php
// $Id$

/**
 * @file
 * Moderation
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 */

/*
 * Menu callback: content administration.
 */
function moderation_node_queue() {
  drupal_add_css(drupal_get_path('module', 'moderation') .'/moderation.css');
  drupal_add_js(drupal_get_path('module', 'moderation') .'/moderation_node.js');

  $query = "SELECT n.*, u.name, u.uid, zm.status as moderate FROM {node} n  
              INNER JOIN {users} u ON n.uid = u.uid
              LEFT JOIN {moderation_moderation} zm ON n.nid = zm.obj_id
                WHERE zm.obj_type = 'node'
                  AND (zm.status IS NULL OR zm.status=0)
                  AND n.type IN ('" . implode('\' ,\'', variable_get('moderation_moderated_types', array())) ."')
                ORDER BY n.created DESC";
  $result = pager_query($query, 50);
  
  $destination = drupal_get_destination();
  
  while ($node = db_fetch_object($result)) {
    $query = "SELECT z.uid, z.created, u.name FROM {moderation} z  
                LEFT JOIN {users} u ON z.uid = u.uid
                WHERE obj_id=%d
                  AND obj_type='node'
                ORDER BY z.created DESC
                LIMIT 1";
    $moderation = db_fetch_object(db_query($query, $node->nid));

    $item = '<div id="moderation-node-'. $node->nid .'" class="moderation-node clear-block container-inline">';
    $item .= '<div id="moderation-title-'. $node->nid .'" class="moderation-title title">';
    $item .= format_date($node->created, 'small') .' ';
    $item .= l($node->title, 'node/'. $node->nid) .' ';
    $item .= theme('mark', node_mark($node->nid, $node->changed));
    if ($moderation) {
      $item .= ' <div class="moderation-moderation" title="'. t('Last status change') .'">'. t('(!user !date)', array('!user' => theme('username', (object) array('uid' => $moderation->uid, 'name' => $moderation->name)), '!date' => format_date($moderation->created, 'small'))) .'</div>';
    }
    $item .= '</div>';
    $item .= '</div>';
    
    $item .= '<div id="moderation-info-'. $node->nid .'" class="moderation-info clear-block">';
    $item .= '  <span id="moderation-operations-'. $node->nid .'" class="moderation-attribute moderation-operations">'. l(t('edit'), 'node/'. $node->nid .'/edit', array('target' => '_blank'), $destination) .'</span>';
    $item .= '  <span id="moderation-username-'. $node->nid .'" class="moderation-attribute moderation-username">'. t('By !user', array('!user' => theme('username', $node))) .'</span>';
    $item .= '  <span id="moderation-name-'. $node->nid .'" class="moderation-attribute moderation-name">'. check_plain(node_get_types('name', $node)) .'</span>';
    $item .= '  <span id="moderation-status-'. $node->nid .'" class="moderation-attribute moderation-status">'. ($node->status   ? t('published') : t('not published')) .'</span>';
    $item .= '  <span id="moderation-promote-'. $node->nid .'" class="moderation-attribute moderation-promote">'. ($node->promote  ? t('promoted')  : t('not promoted')) .'</span>';
    $item .= '  <span id="moderation-sticky-'. $node->nid .'" class="moderation-attribute moderation-sticky">'. ($node->sticky   ? t('sticky')    : t('not sticky')) .'</span>';
    $item .= '  <span id="moderation-moderate-'. $node->nid .'" class="moderation-attribute moderation-moderate">'. ($node->moderate ? t('moderated') : t('not moderated')) .'</span>';
    $item .= '</div>';
    
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
  
  $output .= theme('table', array(), $rows);
  $output .= theme('pager', NULL, 50);
  
  return $output;
}

/**
 * Menu callback; present an administrative comment listing.
 */
function moderation_comment_queue() {
  drupal_add_css(drupal_get_path('module', 'moderation') .'/moderation.css');
  drupal_add_js(drupal_get_path('module', 'moderation') .'/moderation_comment.js');
  
  $result = pager_query("SELECT c.*, zm.status as moderate FROM {comments} c 
                           LEFT JOIN {moderation_moderation} zm ON c.cid = zm.obj_id
                           WHERE zm.obj_type = 'comment'
                             AND (zm.status IS NULL OR zm.status=0)
                           ORDER BY c.timestamp DESC", 50);
  $destination = drupal_get_destination();
  
  while ($comment = db_fetch_object($result)) {
    $query = "SELECT z.uid, z.created, u.name FROM {moderation} z  
                LEFT JOIN {users} u ON z.uid = u.uid
                WHERE z.obj_id=%d
                  AND z.obj_type='comment'
                ORDER BY z.created DESC
                LIMIT 1";
    $moderation = db_fetch_object(db_query($query, $comment->cid));
    
    $item = '<div id="moderation-comment-'. $comment->cid .'" class="moderation-comment clear-block container-inline">';
    $item .= '<div id="moderation-title-'. $comment->cid .'" class="moderation-title title">';
    $item .= format_date($comment->timestamp, 'small') .' ';
    $item .= l($comment->subject, 'node/'. $comment->nid, array(), NULL, 'comment-'. $comment->cid) .' ';
    $item .= theme('mark', node_mark($comment->cid, $comment->changed));
    if ($moderation) {
      $item .= ' <div class="moderation-moderation" title="'. t('Last status change') .'">'. t('(!user !date)', array('!user' => theme('username', (object) array('uid' => $moderation->uid, 'name' => $moderation->name)), '!date' => format_date($moderation->created, 'small'))) .'</div>';
    }
    $item .= '</div>';
    $item .= '</div>';
    
    $item .= '<div id="moderation-info-'. $comment->cid .'" class="moderation-info clear-block">';
    $item .= '  <span id="moderation-operations-'. $comment->cid .'" class="moderation-attribute moderation-operations">'. l(t('edit'), 'comment/edit/'. $comment->cid, array('target' => '_blank'), $destination) .'</span>';
    $item .= '  <span id="moderation-username-'. $comment->cid .'" class="moderation-attribute moderation-username">'. t('By !user', array('!user' => theme('username', $comment))) .'</span>';
    $item .= '  <span id="moderation-moderate-'. $comment->cid .'" class="moderation-attribute moderation-moderate">'. ($comment->moderate ? t('moderated') : t('not moderated')) .'</span>';
    $item .= '  <span id="moderation-status-'. $comment->cid .'" class="moderation-attribute moderation-status">'. ($comment->status   ? t('not published') : t('published')) .'</span>';
    $item .= '</div>';
    
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
  
  $output .= theme('table', array(), $rows);
  $output .= theme('pager', NULL, 50);
  return $output;
}

