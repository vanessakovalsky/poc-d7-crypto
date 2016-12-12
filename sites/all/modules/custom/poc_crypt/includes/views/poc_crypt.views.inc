<?php

/**
 * Implementation of hook_views_query_alter
 * @param type $view
 * @param type $query
 */
function poc_crypt_views_query_alter(&$view, &$query) {
  if ($view->name == 'listes_users') {
    dpm('passe dans le query alter');
    dpm($view->query);
    //$view->query->where[1]['conditions'][1]['value'] = encrypt($view->query->where[1]['conditions'][1]['value']);
    dpm($view->query->where[1]['conditions']);

    //to find out what the fieldname ist: use devel and add your desired field as
    //first filter ( =>orderby[0]). install devel and uncomment the next line
    //dpm($view->query->orderby)
  }
}
