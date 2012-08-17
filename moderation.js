(function($) {

/**
 * Moderation toggle function of the livepreview and the moderate event.
 */
Drupal.behaviors.moderation = {
  attach: function (context, settings) {
    $('.use-ajax', context).click(function() {
      if($(this).hasClass('action')) {
        var action_id = $(this).parent().attr('id').split('_');
        var entity_id = action_id[2];
        var id = 'title_' + entity_id + '_content';
        // Remove content on moderate action.
        $('#' + id).empty();
        $('.open').removeClass('open');
      }
      else {
        var id = $(this).attr('id') + '_content';
        if($(this).hasClass('open')) {
          $(this).removeClass('open');
          // Remove content.
          $('#' + id).empty();
          $('#' + id).attr('id', id + '_removed');
        }
        else {
          $(this).addClass('open');
          $('#' + id + '_removed').attr('id', id);
        }
      }
    });
  }
};

})(jQuery);
