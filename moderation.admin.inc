<?php

/**
 * UI controller.
 */
class ModerationUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Moderations',
      'description' => 'Add edit and update moderations.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
      'type' => MENU_DEFAULT_LOCAL_TASK,
    );

    // Change the overview menu type for the list of moderations.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;

    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a moderation',
      'description' => 'Add a new moderation',
      'page callback' => 'moderation_add_page',
      'access callback' => 'moderation_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'moderation.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    // Add menu items to add each different type of entity.
    foreach (moderation_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'moderation_form_wrapper',
        'page arguments' => array(moderation_create(array('type' => $type->type))),
        'access callback' => 'moderation_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'moderation.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing moderation entities
    $items[$this->path . '/moderation/' . $wildcard] = array(
      'page callback' => 'moderation_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'moderation_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'moderation.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/moderation/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    $items[$this->path . '/moderation/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'moderation_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'moderation_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'moderation.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    // Menu item for viewing moderations
    $items['moderation/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'moderation_page_title',
      'title arguments' => array(1),
      'page callback' => 'moderation_page_view',
      'page arguments' => array(1),
      'access callback' => 'moderation_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }

  /**
   * Create the markup for the add Moderation Entities page within the class
   * so it can easily be extended/overriden.
   */
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }

    return theme('moderation_add_list', array('content' => $content));
  }

}

/**
 * Form callback wrapper: create or edit a moderation.
 *
 * @param $moderation
 *   The moderation object being edited by this form.
 *
 * @see moderation_edit_form()
 */
function moderation_form_wrapper($moderation) {
  // Add the breadcrumb for the form's location.
  moderation_set_breadcrumb();
  return drupal_get_form('moderation_edit_form', $moderation);
}

/**
 * Form callback wrapper: delete a moderation.
 *
 * @param $moderation
 *   The moderation object being edited by this form.
 *
 * @see moderation_edit_form()
 */
function moderation_delete_form_wrapper($moderation) {
  // Add the breadcrumb for the form's location.
  //moderation_set_breadcrumb();
  return drupal_get_form('moderation_delete_form', $moderation);
}

/**
 * Form callback: create or edit a moderation.
 *
 * @param $moderation
 *   The moderation object to edit or for a create form an empty moderation object
 *     with only a moderation type defined.
 */
function moderation_edit_form($form, &$form_state, $moderation) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Moderation Name'),
    '#default_value' => isset($moderation->name) ? $moderation->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );

  $form['data']['#tree'] = TRUE;
  $form['data']['sample_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('An interesting moderation switch'),
    '#default_value' => isset($moderation->data['sample_data']) ? $moderation->data['sample_data'] : 1,
  );

  // Add the field related form elements.
  $form_state['moderation'] = $moderation;
  field_attach_form('moderation', $moderation, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save moderation'),
    '#submit' => $submit + array('moderation_edit_form_submit'),
  );

  if (!empty($moderation->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete moderation'),
      '#suffix' => l(t('Cancel'), 'admin/content/moderations'),
      '#submit' => $submit + array('moderation_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'moderation_edit_form_validate';
  return $form;
}

/**
 * Form API validate callback for the moderation form
 */
function moderation_edit_form_validate(&$form, &$form_state) {
  $moderation = $form_state['moderation'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('moderation', $moderation, $form, $form_state);
}

/**
 * Form API submit callback for the moderation form.
 *
 * @todo remove hard-coded link
 */
function moderation_edit_form_submit(&$form, &$form_state) {

  $moderation = entity_ui_controller('moderation')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the moderation and go back to the list of moderations
  // Add in created and changed times.
  if ($moderation->is_new = isset($moderation->is_new) ? $moderation->is_new : 0) {
    $moderation->created = time();
  }

  $moderation->changed = time();

  $moderation->save();
  $form_state['redirect'] = 'admin/content/moderations';
}

/**
 * Form API submit callback for the delete button.
 *
 * @todo Remove hard-coded path
 */
function moderation_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/moderations/moderation/' . $form_state['moderation']->moderation_id . '/delete';
}

/**
 * Form callback: confirmation form for deleting a moderation.
 *
 * @param $moderation
 *   The moderation to delete
 *
 * @see confirm_form()
 */
function moderation_delete_form($form, &$form_state, $moderation) {
  $form_state['moderation'] = $moderation;

  $form['#submit'][] = 'moderation_delete_form_submit';

  $form = confirm_form($form, t('Are you sure you want to delete moderation %name?', array('%name' => $moderation->name)), 'admin/content/moderations/moderation', '<p>' . t('This action cannot be undone.') . '</p>', t('Delete'), t('Cancel'), 'confirm'
  );

  return $form;
}

/**
 * Submit callback for moderation_delete_form
 */
function moderation_delete_form_submit($form, &$form_state) {
  $moderation = $form_state['moderation'];

  moderation_delete($moderation);

  drupal_set_message(t('The moderation %name has been deleted.', array('%name' => $moderation->name)));
  watchdog('moderation', 'Deleted moderation %name.', array('%name' => $moderation->name));

  $form_state['redirect'] = 'admin/content/moderations';
}

/**
 * Page to add Moderation Entities.
 *
 * @todo Pass this through a proper theme function
 */
function moderation_add_page() {
  $controller = entity_ui_controller('moderation');
  return $controller->addPage();
}

/**
 * Displays the list of available moderation types for moderation creation.
 *
 * @ingroup themeable
 */
function theme_moderation_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="moderation-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer moderation types')) {
      $output = '<p>' . t('Moderation Entities cannot be added because you have not created any moderation types yet. Go to the <a href="@create-moderation-type">moderation type creation page</a> to add a new moderation type.', array('@create-moderation-type' => url('admin/structure/moderation_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No moderation types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}

/**
 * Sets the breadcrumb for administrative moderation pages.
 */
function moderation_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Moderations'), 'admin/content/moderations'),
  );

  drupal_set_breadcrumb($breadcrumb);
}
