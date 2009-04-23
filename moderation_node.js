Drupal.moderationInit = function() {
  var nid = $(this).id().substring(16);
  
  $(this).after('<div id="moderation-preview-'+nid+'" class="moderation-preview"></div>');
  $('#moderation-preview-'+nid).hide();
}

Drupal.moderationPreview = function() {
  $(this).click(function () {
    var nid = $(this).parent().id().substring(17);
    var preview = $('#moderation-preview-'+nid);

    if (preview.html().length == 0) {
      //load node data
      var html = $.ajax({
        url: "?q=moderation/node/"+nid+"/get/preview",
        dataType: "json",
        success: function(html){
          preview.html(html);
        }
      });
    }
    preview.toggle();

    $.ajax({
      url: "?q=moderation/node/"+nid+"/get/moderate",
      dataType: "json",
      success: function(status) {
        $('#moderation-preview-'+nid+' .moderation-messages').remove();
        if (status == 1) {
          $('#moderation-preview-'+nid).prepend('<div class="moderation-messages status">This item has been moderated in the meantime.</div>');
        }
      }
    });

    return false;
  });
}

Drupal.moderationButtonStatus = function() {
  var nid = $(this).id().substring(18);

  //insert buttons
  if ($(this).id().match('moderation-status-')) $(this).html('<a id="moderation-status-link-'+nid+'" href="#">'+$(this).html()+'</a>');
  $('#moderation-status-link-'+nid).click(function () {
    //load node data
    var html = $.ajax({
      url: "?q=moderation/node/"+nid+"/set/status",
      dataType: "json",
      success: function(result){
        if (result[0]) {
          if (result[1]) $('#moderation-status-link-'+nid).html('published');
          else $('#moderation-status-link-'+nid).html('not published');
        }
      }
    });
    return false;
  });
}

Drupal.moderationButtonPromote = function() {
  var nid = $(this).id().substring(19);

  //insert buttons
  if ($(this).id().match('moderation-promote-')) $(this).html('<a id="moderation-promote-link-'+nid+'" href="#">'+$(this).html()+'</a>');
  $('#moderation-promote-link-'+nid).click(function () {
    //load node data
    var html = $.ajax({
      url: "?q=moderation/node/"+nid+"/set/promote",
      dataType: "json",
      success: function(result){
        if (result[0]) {
          if (result[1]) $('#moderation-promote-link-'+nid).html('promoted');
          else $('#moderation-promote-link-'+nid).html('not promoted');
        }
      }
    });
    return false;
  });
}

Drupal.moderationButtonSticky = function() {
  var nid = $(this).id().substring(18);
  
  //insert buttons
  if ($(this).id().match('moderation-sticky-')) $(this).html('<a id="moderation-sticky-link-'+nid+'" href="#">'+$(this).html()+'</a>');
  $('#moderation-sticky-link-'+nid).click(function () {
    //load node data
    var html = $.ajax({
      url: "?q=moderation/node/"+nid+"/set/sticky",
      dataType: "json",
      success: function(result){
        if (result[0]) {
          if (result[1]) $('#moderation-sticky-link-'+nid).html('sticky');
          else $('#moderation-sticky-link-'+nid).html('not sticky');
        }
      }
    });
    return false;
  });
}

Drupal.moderationButtonModerate = function() {
  var nid = $(this).id().substring(20);

  //insert buttons
  if ($(this).id().match('moderation-moderate-')) $(this).html('<a id="moderation-moderate-link-'+nid+'" href="#">'+$(this).html()+'</a>');
  $('#moderation-moderate-link-'+nid).click(function () {
    //load node data
    var html = $.ajax({
      url: "?q=moderation/node/"+nid+"/set/moderate",
      dataType: "json",
      success: function(result){
        if (result[0]) {
          if (result[1]) {
            $('#moderation-moderate-link-'+nid).html('moderated');
            $('#moderation-preview-'+nid+':visible').slideUp();
          }
          else {
            $('#moderation-moderate-link-'+nid).html('not moderated');
          }
        }
      }
    });
    return false;
  });
}

// Global Killswitch
if (Drupal.jsEnabled) {
  $(document).ready(function() {
    $('.moderation-info').each(Drupal.moderationInit);
    $('.moderation-node .title a').each(Drupal.moderationPreview);

    $('.moderation-status').each(Drupal.moderationButtonStatus);
    $('.moderation-promote').each(Drupal.moderationButtonPromote);
    $('.moderation-sticky').each(Drupal.moderationButtonSticky);
    $('.moderation-moderate').each(Drupal.moderationButtonModerate);
    
    $('.moderation-moderate, .moderation-sticky, .moderation-promote, .moderation-status').show();
  });
}
