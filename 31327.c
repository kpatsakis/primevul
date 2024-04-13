insert_nameinfo (DBusGProxyManager       *manager,
		 const char              *owner,
		 DBusGProxyNameOwnerInfo *info)
{
  GSList *names;
  gboolean insert;

  names = g_hash_table_lookup (manager->owner_names, owner);

  /* Only need to g_hash_table_insert the first time */
  insert = (names == NULL);

  names = g_slist_append (names, info); 

  if (insert)
    g_hash_table_insert (manager->owner_names, g_strdup (owner), names);
}
