Moderation
http://drupal.org/project/moderation
====================================


DESCRIPTION
-----------
It allows moderators to review all posted content while already published.
Included Ajax based functionality allows the moderators to preview the content 
and to change status like published, moderated, promoted, sticky without having to 
leave the page.
A seperate page provides a log, listing all status changes with who and when it was changed.


REQUIREMENTS
------------
Drupal 7.x
entity 7.x-1.x - http://drupal.org/project/entity/git-instructions
views - http://drupal.org/project/views
rules - http://drupal.org/project/rules
rules_conditional - http://drupal.org/project/rules_conditional


INSTALLING
----------
1. To install the module copy the 'livereload' folder to your sites/all/modules
   directory.
2  Before enableling, you need to apply the following patch to entity 7.x-1.x.
   http://drupal.org/node/1435418#comment-6063078
   Attention! Not applying this patch will cause a sitewide error.
3. Read more about installing modules at http://drupal.org/node/70151


UPGRADING
---------
Moderation offers you an upgradepath from D6 to D7. Follow the upgrade instructions
in UPGRADE.txt
IMPORTANT! In order to work, the upgradepath needs the devel module. Please change this lines in
moderation.info to:
;Currently is devel depended, this should be undone sometime.
;dependencies[] = devel
-----------------------
;Currently is devel depended, this should be undone sometime.
dependencies[] = devel

If you dont do that, your installation will break!


CONFIGURING AND USING
---------------------
1. Go to admin/structure/moderation_types and klick on "Add Moderation" to add a new
   moderation queue.
2. Select your queue preferences, klick "Save moderation type", and your queue is ready to use!


REPORTING ISSUE. REQUESTING SUPPORT. REQUESTING NEW FEATURE
-----------------------------------------------------------
1. Go to the module issue queue at http://drupal.org/project/issues/moderation?categories=All
2. Click on CREATE A NEW ISSUE link.
3. Fill the form.
4. To get a status report on your request go to http://drupal.org/project/issues/user


AUTHOR/MAINTAINER
=================
Stefan Auditor (Sanduhrs) <stefan.auditor at erdfisch DOT de>
Pascal Crott (Hydra) <drupal at pascalcrott DOT de>
http://erdfisch.de