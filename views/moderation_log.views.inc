<?php

/**
 * Implements hook_views_data().
 */
function moderation_log_views_data() {
    $data = array();
    $data['moderation_log']['table'] = array(
        'group' => t("Moderation log"),
        'title' => t("Moderation logs"),
        'help' => t("Shows social stream items targeted at the current user."),
    );
    $data['moderation_log']['table']['base'] = array(
        'field' => 'moderation_log_id',
        'title' => 'Moderation Log',
        'help' => t("A moderation log queue."),
        'weight' => 10,
    );
    $data['moderation_log']['moderation_log_id'] = array(
        'title' => t('Moderation log id'),
        'help' => t('The unique moderaion log id.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
    );
    $data['moderation_log']['event'] = array(
        'title' => t('Action'),
        'help' => t('The event proceeded.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
    );
    $data['moderation_log']['entity_id'] = array(
        'title' => t('Entity id'),
        'help' => t('The entity id of the moderated subject.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
    );
    $data['moderation_log']['entity_type'] = array(
        'title' => t('Entity type'),
        'help' => t('The entity type of the moderated subject.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
    );
    $data['moderation_log']['bundle'] = array(
        'title' => t('Entity bundle'),
        'help' => t('The bundle of the moderated subject.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );

    return $data;
}
