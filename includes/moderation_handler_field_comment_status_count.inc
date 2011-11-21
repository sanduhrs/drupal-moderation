<?php
class moderation_handler_field_comment_status_count extends views_handler_field {

  private $comment_count_status = array();

  function allow_advanced_render() {
    return FALSE;
  }

  function query() {
    // do nothing -- to override the parent query.
  }

  /**
   * Fetchees the number of comments by status for each thread.
   *
   * @param array $values
   */
  function pre_render($values) {
    $nids = array();

    foreach ($values as $node) {
      $nids[] = $node->nid;
    }

    $placeholders = db_placeholders($nids);
    $query = "SELECT COUNT(cid) AS comment_count_status, nid
      FROM {comments} c JOIN {moderation_comment_status} m USING (cid)
      WHERE m.status = 0 AND nid IN ($placeholders) GROUP BY nid";
    $result = db_query($query, $nids);

    while ($row = db_fetch_array($result)) {
      $this->comment_count_status[$row['nid']] = $row['comment_count_status'];
    }
  }

  /**
   * Returns the number of comments by status for each thread.
   *
   * @param object $values
   * @return string
   */
  function render($values) {
    return check_plain($this->comment_count_status[$values->nid]);
  }

}