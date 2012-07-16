<?php

/**
 * Implements hook_default_rules_configuration().
 * TODO: make this without the import.
 */
function moderation_default_rules_configuration() {
  $configs = array();
  foreach (moderation_action_list() as $entity_type => $action) {
    foreach ($action as $key => $value) {
      if (!in_array($key, $configs) && isset($value['action'])) {
        $event = isset($value['event']) ? $value['event'] : 'moderation_on_moderate';
        $rule = rules_import('
        { "' . $key . '" : {
            "LABEL" : "' . $value['event'] . '",
            "PLUGIN" : "reaction rule",
            "REQUIRES" : [ "rules", "devel", "rules_conditional", "moderation" ],
            "ON" : [ "' . $event . '" ],
            "IF" : [ { "entity_is_of_type" : { "entity" : [ "entity" ], "type" : "' . $entity_type . '" } } ],
            "DO" : [
              { "CONDITIONAL" : [
                  {
                    "IF" : { "data_is" : { "data" : [ "entity:' . $value['action']['if']['data'][0] . '" ], "value" : "' . $value['action']['if']['data'][1] . '" } },
                    "DO" : [ { "data_set" : { "data" : [ "entity:' . $value['action']['if']['do'][0] . '" ], "value" : "' . $value['action']['if']['do'][1] . '" } } ]
                  },
                  {
                    "ELSE IF" : { "data_is" : { "data" : [ "entity:' . $value['action']['else if']['data'][0] . '" ], "value" : "' . $value['action']['else if']['data'][1] . '" } },
                    "DO" : [ { "data_set" : { "data" : [ "entity:' . $value['action']['else if']['do'][0] . '" ], "value" : "' . $value['action']['else if']['do'][1] . '" } } ]
                  }
                ]
              }
            ]
          }
        }
        ');
        $configs[$rule->name] = $rule;
      }
    }
  }
  // Moderation Rule.
  $rule = rules_import('
    { "rules_moderate" : {
      "LABEL" : "moderate",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "moderation" ],
      "ON" : [ "moderation_on_moderate" ],
      "IF" : [
        { "NOT entity_is_of_type" : { "entity" : [ "entity" ], "type" : "moderation" } }
      ],
      "DO" : [ { "moderations_action_moderate" : { "entity" : [ "entity" ] } } ]
    }
  }
  ');
  $configs[$rule->name] = $rule;

  return $configs;
}
