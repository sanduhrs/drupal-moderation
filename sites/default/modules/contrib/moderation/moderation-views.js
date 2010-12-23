/**
 * @TODO
 *    Figure out how to reuse the moderation js.
 */
(function($) {
  Drupal.moderationPreview = function(context) {
  $("a.moderation-ajax-preview:not('.moderation-preview-processed')", context)
  .after('<div class="moderation-preview"></div>')
  .click(function() {
    var link = $(this);
    var obj_id = $(this).siblings('div.moderation-preview-attributes').attr('entity-id');
    var type = $(this).siblings('div.moderation-preview-attributes').attr('type');

    var preview = $(this).siblings('div.moderation-preview');
    var url = window.location.protocol+'//'+window.location.hostname+Drupal.settings.basePath+"?q=moderation/" + type + "/" + obj_id + "/get/preview";
    console.log(url);
    if (preview.html().length == 0) {
      link.addClass('throbbing');

      // load preview data
      var html = $.ajax({
        url: url+'&js=1',
        dataType: "json",
        success: function(html) {
          preview.html(html);
          Drupal.attachBehaviors(preview);
          link.removeClass('throbbing');
          preview.show();
        }
      });
    }

    preview.toggle();

    $.ajax({
      url: window.location.protocol+'//'+window.location.hostname+Drupal.settings.basePath+"?q=moderation/" + type + "/"+obj_id+"/get/moderate&js=1",
      dataType: "json",
      success: function(status) {
        preview.find('.moderation-messages').remove();
        if (status == 1) {
          preview.prepend('<div class="moderation-messages status">'+Drupal.t('This item has been moderated in the meantime.')+'</div>');
        }
      }
    });

    return false;
  })
  .addClass('moderation-preview-processed');
};

Drupal.behaviors.moderationPreview = function(context) {
  Drupal.moderationPreview(context);
};

Drupal.moderationButton = function() {
  var id = $(this).attr("id");
  var type = id.split('-')[1]
  var obj_id = id.split('-')[3];
  var url = window.location.protocol+'//'+window.location.hostname+$(this).attr("href");
  var text = {
    'status': {0: Drupal.t('publish'), 1: Drupal.t('unpublish')},
    'promote': {0: Drupal.t('promote'), 1: Drupal.t('demote')},
    'sticky': {0: Drupal.t('make sticky'), 1: Drupal.t('remove stickiness')},
    'moderate': {0: Drupal.t('moderate'), 1: Drupal.t('unmoderate')}
  };

  $(this).click(function () {
    var link = $(this);
    link.addClass('throbbing');

    //load node data
    var html = $.ajax({
      url: url+'&js=1',
      dataType: "json",
      success: function(result){
        // Exception for comments status
        if (result[0] && !(result[3] == 'comment' && type == 'status')) {
          if (result[1]) $('#'+id).html(text[type][1]);
          else $('#'+id).html(text[type][0]);
        }
        else {
          if (result[1]) $('#'+id).html(text[type][0]);
          else $('#'+id).html(text[type][1]);
        }
        if (result[1] && type == 'moderate') {
          $('#'+id).parent().parent().parent().next('.moderation-preview').hide('slow');
        }
        link.removeClass('throbbing');
      }
    });
    return false;
  });
};

Drupal.behaviors.moderationInit = function (context) {
  $('.moderation-content', context).each(Drupal.moderationPreview);
  $('.moderation-status-link', context).each(Drupal.moderationButton);
  $('.moderation-promote-link', context).each(Drupal.moderationButton);
  $('.moderation-sticky-link', context).each(Drupal.moderationButton);
  $('.moderation-moderate-link', context).each(Drupal.moderationButton);
};

})(jQuery);