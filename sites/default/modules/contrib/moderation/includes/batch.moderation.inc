<?php
// $Id$

function moderation_batch_import(&$context) {
  // Use the $context['sandbox'] at your convenience to store the
  // information needed to track progression between successive calls.
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_cid'] = 0;
    $context['sandbox']['max'] = db_result(db_query('SELECT COUNT(DISTINCT cid) FROM {comments}'));
  }

  // Process comments by groups of 5 (arbitrary value).
  // When a group of five is processed, the batch update engine determines
  // whether it should continue processing in the same request or provide
  // progress feedback to the user and wait for the next request.
  $limit = 5;

  // Retrieve the next group of nids.
  $result = db_query_range("SELECT cid FROM {comments} WHERE cid > %d ORDER BY cid ASC", $context['sandbox']['current_cid'], 0, $limit);
  while ($row = db_fetch_array($result)) {
    $comment = _comment_load($row['cid']);
    $record = array(
      'cid' => $comment->cid,
      'status' => 0,
    );
    drupal_write_record('moderation_comment_status', $record);

    // Store some result for post-processing in the finished callback.
    $context['results'][] = $comment->cid . ' : ' . check_plain($comment->subject);

    // Update our progress information.
    $context['sandbox']['progress']++;
    $context['sandbox']['current_cid'] = $comment->cid;
    $context['message'] = check_plain($comment->subject);
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

function moderation_batch_import_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural(count($results), 'One comment processed.', '@count comments processed.');
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
  // Providing data for the redirected page is done through $_SESSION.
  foreach ($results as $result) {
    $items[] = t('Loaded comment %subject.', array('%subject' => $result));
  }
  $_SESSION['moderation_batch_results'] = $items;
}
