// $Id: moderation.js,v 1.1.2.10 2010/07/09 08:19:58 sanduhrs Exp $
(function($) {
Drupal.moderationPreview = function() {
  var id = $(this).attr("id");
  var obj_id = $(this).attr("id").split('-')[2];
  $(this).next('.moderation-info').after('<div id="moderation-preview-'+obj_id+'" class="moderation-preview"></div>');
  
  $('.moderation-title > a', this).click(function () {
    var link = $(this);

	var preview = $('#moderation-preview-'+obj_id);
    var url = window.location.protocol+'//'+window.location.hostname+(window.location.port ? ':'+window.location.port : '')+$(this).attr("href");
    if (preview.html().length == 0) {
      link.addClass('throbbing');

      //load node data
      var html = $.ajax({
        url: url+'&js=1',
        dataType: "json",
        success: function(html){
          preview.html(html);
          Drupal.attachBehaviors(preview);
          link.removeClass('throbbing');
        }
      });
    }

    preview.toggle();

    $.ajax({
      url: window.location.protocol+'//'+window.location.hostname+Drupal.settings.basePath+"?q=moderation/"+Drupal.settings.moderationType+"/"+obj_id+"/get/moderate&js=1",
      dataType: "json",
      success: function(status) {
        $('#moderation-preview-'+obj_id+' .moderation-messages').remove();
        if (status == 1) {
          $('#moderation-preview-'+obj_id).prepend('<div class="moderation-messages status">'+Drupal.t('This item has been moderated in the meantime.')+'</div>');
        }
      }
    });
    
    return false;
  });
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
