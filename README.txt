Moderation
http://drupal.org/project/moderation
====================================


DESCRIPTION
-----------
The moderation module provides a node and a comment queue.

It allows moderators to review all posted content while already published.
Included Ajax based functionality allows the moderators to preview the content and to change status like published, moderated, promoted, sticky without having to leave the page. A seperate page provides a log, listing all status changes with who and when it was changed.

This project was developed by erdfisch (http://erdfisch.de) for ZEIT Online (http://www.zeit.de).


REQUIREMENTS
------------
Drupal 6.x


INSTALLING
----------
1. To install the module copy the 'moderation' folder to your sites/all/modules directory.

2. Go to admin/build/modules. Enable the module.
Read more about installing modules at http://drupal.org/node/70151


CONFIGURING AND USING
---------------------
1. Go to admin/settings/moderation. Set nodetypes that you want to be moderated. Click on 'Save configuration' button. 

2. To start moderating CONTENT go to admin/content/node. Click on 'Moderation' tab. Click one of the following links beside each content:  Moderate, Make sticky, Demote, Unpublish, Edit. To clarify the location of those links view the following screenshot at http://drupal.org/node/442728

3. To start moderating COMMENTS go to admin/content/comment. Click on 'Moderation' tab. Click one of the following links beside each comment:  Publish, moderate, edit.


REPORTING ISSUE. REQUESTING SUPPORT. REQUESTING NEW FEATURE
-----------------------------------------------------------
1. Go to the module issue queue at http://drupal.org/project/issues/moderation?status=All&categories=All
2. Click on CREATE A NEW ISSUE link.
3. Fill the form.
4. To get a status report on your request go to http://drupal.org/project/issues/user


UPGRADING
---------
Read more at http://drupal.org/node/250790

// $Id$