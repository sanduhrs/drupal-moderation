<?php

/**
 * Implementation of hook_views_default_views().
 */
function moderation_log_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'moderation_logs';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'moderation_log';
  $view->human_name = 'Moderation logs';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Moderation logs';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'moderation_log_id' => 'moderation_log_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'moderation_log_id' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Permission settings */
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'moderation logs';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'Currently no logs available.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Moderation log: Moderation log id */
  $handler->display->display_options['fields']['moderation_log_id']['id'] = 'moderation_log_id';
  $handler->display->display_options['fields']['moderation_log_id']['table'] = 'moderation_log';
  $handler->display->display_options['fields']['moderation_log_id']['field'] = 'moderation_log_id';
  /* Field: Moderation log: User */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'moderation_log';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  /* Field: Moderation log: Action */
  $handler->display->display_options['fields']['event']['id'] = 'event';
  $handler->display->display_options['fields']['event']['table'] = 'moderation_log';
  $handler->display->display_options['fields']['event']['field'] = 'event';
  /* Field: Moderation log: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'moderation_log';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'long';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'admin/moderations/moderation_log';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Moderation Logs';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;

  $views[$view->name] = $view;
  return $views;
}
