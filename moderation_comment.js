Drupal.moderationInit = function() {
  var cid = $(this).id().substring(16);
  $(this).after('<div id="moderation-preview-'+cid+'" class="moderation-preview"></div>');
  $('#moderation-preview-'+cid).hide();
};

Drupal.moderationPreview = function() {
  $(this).click(function () {
    var cid = $(this).parent().id().substring(17);
    var preview = $('#moderation-preview-'+cid);

    if (preview.html().length == 0) {
      //load node data
      var html = $.ajax({
        url: "?q=moderation/comment/"+cid+"/get/preview",
        dataType: "json",
        success: function(html){
          preview.html(html);
        }
      });
    }
    preview.toggle();

    $.ajax({
      url: "?q=moderation/comment/"+cid+"/get/moderate",
      dataType: "json",
      success: function(status) {
        $('#moderation-preview-'+cid+' .moderation-messages').remove();
        if (status == 1) {
          $('#moderation-preview-'+cid).prepend('<div class="moderation-messages status">This item has been moderated in the meantime.</div>');
        }
      }
    });

    return false;
  });
};

Drupal.moderationButtonStatus = function() {
  var cid = $(this).id().substring(18);

  //insert buttons
  if ($(this).id().match('moderation-status-')) $(this).html('<a id="moderation-status-link-'+cid+'" href="?q=moderation/comment/status/'+cid+'">'+$(this).html()+'</a>');
  $('#moderation-status-link-'+cid).click(function () {
    //load node data
    var html = $.ajax({
      url: "?q=moderation/comment/"+cid+"/set/status",
      dataType: "json",
      success: function(result){
        if (result[0]) {
          if (result[1]) $('#moderation-status-link-'+cid).html('published');
          else $('#moderation-status-link-'+cid).html('not published');
        }
      }
    });
    return false;
  });
};

Drupal.moderationButtonModerate = function() {
  var cid = $(this).id().substring(20);

  //insert buttons
  if ($(this).id().match('moderation-moderate-')) $(this).html('<a id="moderation-moderate-link-'+cid+'" href="?q=moderation/comment/moderate/'+cid+'">'+$(this).html()+'</a>');
  $('#moderation-moderate-link-'+cid).click(function () {
    //load node data
    var html = $.ajax({
      url: "?q=moderation/comment/"+cid+"/set/moderate",
      dataType: "json",
      success: function(result){
        if (result[0]) {
          if (result[1]) {
            $('#moderation-moderate-link-'+cid).html('moderated');
            $('#moderation-preview-'+cid+':visible').slideUp();
          }
          else {
            $('#moderation-moderate-link-'+cid).html('not moderated');
          }
        }
      }
    });
    return false;
  });
};

// Global Killswitch
if (Drupal.jsEnabled) {
  $(document).ready(function() {
    $('.moderation-info').each(Drupal.moderationInit);
    $('.moderation-comment .title a').each(Drupal.moderationPreview);
    
    $('.moderation-status').each(Drupal.moderationButtonStatus);
    $('.moderation-moderate').each(Drupal.moderationButtonModerate);
    
    $('.moderation-moderate, .moderation-sticky, .moderation-promote, .moderation-status').show();
  });
};
