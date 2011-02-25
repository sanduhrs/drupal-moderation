
/**
 * @file
 * Moderation - nodes and comments queue
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 */

(function($) {
  Drupal.behaviors.moderationPreview = function(context) {
    Drupal.moderationPreview(context);
    Drupal.moderationButtons(context);
  };

  Drupal.moderationPreview = function(context) {
    $("a.moderation-ajax-preview:not('.moderation-processed')", context).
    addClass('moderation-processed').
    parent().parent().parent().each(function(index) {
      var wrapper = $(this);
      wrapper.append('<div class="moderation-preview"></div>');
      var type     = $('div.moderation-preview-attributes', wrapper).attr('type');
      var obj_id   = $('div.moderation-preview-attributes', wrapper).attr('entity-id');
      var preview  = $('.moderation-preview', wrapper);
      var base_url = window.location.protocol+'//'+window.location.hostname+(window.location.port ? ':'+window.location.port : '')+Drupal.settings.moderationPath;

      $('a.moderation-ajax-preview', wrapper).click(function(event) {
        event.preventDefault();
        var link = $(this);

        if (preview.html().length == 0) {
          link.addClass('throbbing');

          // load preview data
          var html = $.ajax({
            url: base_url+"/" + type + "/" + obj_id + "/get/preview&js=1",
            dataType: "json",
            success: function(response) {
              preview.append(response);
              Drupal.attachBehaviors(preview);
              link.removeClass('throbbing');
              preview.show();
            }
          });
        }

        preview.toggle();

        $.ajax({
          url: base_url+"/" + type + "/"+obj_id+"/get/moderate&js=1",
          dataType: "json",
          success: function(response) {
            preview.find('.moderation-messages').remove();
            if (response[0] == 1) {
              preview.prepend('<div class="moderation-messages status">'+Drupal.t('This item has already been moderated.')+'</div>');
            }
          }
        });

      })
    });
  };

  Drupal.moderationButtons = function(context) {
    $("a.moderation-status-link, a.moderation-promote-link, a.moderation-sticky-link, a.moderation-moderate-link", context).
    addClass('moderation-processed').
    each(function(index) {
      var id       = $(this).attr("id");
      var type     = id.split('-')[1]
      var obj_id   = id.split('-')[3];
      var base_url = window.location.protocol+'//'+window.location.hostname+(window.location.port ? ':'+window.location.port : '');
      var url      = base_url+$(this).attr("href");

      var text = {
        'status': {
          0: Drupal.t('publish'),
          1: Drupal.t('unpublish')
        },
        'promote': {
          0: Drupal.t('promote'),
          1: Drupal.t('demote')
        },
        'sticky': {
          0: Drupal.t('make sticky'),
          1: Drupal.t('remove stickiness')
        },
        'moderate': {
          0: Drupal.t('moderate'),
          1: Drupal.t('unmoderate')
        }
      };

      $(this).click(function (event) {
        event.preventDefault();
        var link = $(this);
        link.addClass('throbbing');

        //load node data
        var html = $.ajax({
          url: url+'&js=1',
          dataType: "json",
          success: function(response) {
            // Exception for comments status
            if (response[0] && !(response[3] == 'comment' && type == 'status')) {
              if (response[1]) $('#'+id).html(text[type][1]);
              else $('#'+id).html(text[type][0]);
            }
            else {
              if (response[1]) $('#'+id).html(text[type][0]);
              else $('#'+id).html(text[type][1]);
            }
            // Hide preview on un/moderation
            if (type == 'moderate') {
              $('#'+id).parent().parent().parent().parent().parent().find('.moderation-preview').hide('slow');
            }
            link.removeClass('throbbing');
          }
        });
      });
    });
  };
})(jQuery);
