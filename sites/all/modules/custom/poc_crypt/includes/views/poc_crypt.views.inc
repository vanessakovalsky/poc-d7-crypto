<?php

/**
 * Implementation of hook_views_query_alter
 * @param type $view
 * @param type $query
 */
function poc_crypt_views_query_alter(&$view, &$query) {
  if ($view->name == 'listes_users') {
    // if(isset($query->where['1']['conditions'])){
    //   //Récupérer le nom du champs et la valeur passé
    //   $field_name = array();
    //   foreach($view->exposed_raw_input as $field_raw_name => $value_field_raw){
    //     $real_name = substr($field_raw_name, 0, -6);
    //     $field_name[$real_name] = $value_field_raw;
    //   }
    //   //Appeler une fonction qui a partir de la valeur non chiffré et d'une EntityQuery, avec un decrypt renvoit l'uid du user concerné
    //   $uid_user = FALSE;
    //   foreach($field_name as $key_field_name => $value_field_name){
    //     if($value_field_name){
    //       $uid_user = crypt_user_find_by_field($field_name);
    //     }
    //   }
    //   //MOdifier la query, pour remplacer la condition de recherche par un where uid= l'uid renvoyé par la fonction précédente.
    //   if(!empty($uid_user)){
    //     unset($query->where['1']['conditions'][1]);
    //     $query->where['1']['conditions'][1] = [
    //       'field' => 'users.uid',
    //       'value' => $uid_user,
    //       'operator' => '=',
    //     ];
    //   }
    // }
  }
}
