<?php
// $Id$


/**
 * @file
 * Provide integration for rules
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 */

/**
 * Implementation of hook_rules_condition_info().
 */
function moderation_rules_condition_info() {
  return array(
    'moderation_rules_condition_in_moderation' => array(
      'label' => t('In moderation'),
      'arguments' => array(
        'node' => array('type' => 'node', 'label' => t('Content')),
      ),
      'module' => 'Moderation',
    ),
  );
}

function moderation_rules_condition_in_moderation($node) {
  return !$node->moderate;
}

/**
 * Implementation of hook_rules_action_info().
 */
function moderation_rules_action_info() {
  return array(
    'moderation_rules_action_node_moderate' => array(
      'label' => t('Moderate'),
      'arguments' => array(
        'node' => array('type' => 'node', 'label' => t('Content')),
      ),
      'module' => 'Moderation',
    ),
    'moderation_rules_action_node_unmoderate' => array(
      'label' => t('Unmoderate'),
      'arguments' => array(
        'node' => array('type' => 'node', 'label' => t('Content')),
      ),
      'module' => 'Moderation',
    ),
  );
}

function moderation_rules_action_node_moderate(&$node) {
  $node->moderate = 1;
  return array('node' => $node);
}

function moderation_rules_action_node_unmoderate(&$node) {
  $node->moderate = 0;
  return array('node' => $node);
}
