<?php

/**
 * @file
 * Moderation - un/install procedures
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 */

function moderation_admin_settings() {
  $form['thread'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thread queue'),
    '#description' => t('These are the settings for the !link.', array('!link' => l(t('Thread Moderation Queue'), 'admin/content/moderation'))),
  );
  $form['thread']['moderation_thread_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Moderate'),
    '#options' => node_get_types('names'),
    '#default_value' => variable_get('moderation_thread_types', array()),
    '#description' => t('Select the node types you wish to put into the moderation queue.'),
  );
  $form['node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node queue'),
    '#description' => t('These are the settings for the !link.', array('!link' => l(t('Node Moderation Queue'), 'admin/content/node/moderation'))),
  );
  $form['node']['moderation_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Moderate'),
    '#options' => node_get_types('names'),
    '#default_value' => variable_get('moderation_node_types', array()),
    '#description' => t('Select the node types you wish to put into the moderation queue.'),
  );
  $form['comment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment queue'),
    '#description' => t('There is also a !link.', array('!link' => l(t('Comment Moderation Queue'), 'admin/content/comment/moderation'))),
  );
  return system_settings_form($form);
}

/**
 * Only saves contentypes which are checked.
 */
function moderation_admin_settings_validate(&$form, &$form_state) {
  foreach ($form_state['values']['moderation_thread_types'] as $type => $value) {
    if (!$value) {
      unset($form_state['values']['moderation_thread_types'][$type]);
    }
  }
  foreach ($form_state['values']['moderation_node_types'] as $type => $value) {
    if (!$value) {
      unset($form_state['values']['moderation_node_types'][$type]);
    }
  }
}
