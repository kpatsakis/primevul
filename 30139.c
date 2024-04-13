foreach_item_helper (GHashTable *hash,
                     NMVPNIterFunc func,
                     gpointer user_data)
{
	GList *keys, *liter;
	GSList *copied = NULL, *siter;

	g_return_if_fail (hash != NULL);

	/* Grab keys and copy them so that the callback func can modify
	 * the hash table items if it wants to.
	 */
	keys = g_hash_table_get_keys (hash);
	for (liter = keys; liter; liter = g_list_next (liter))
		copied = g_slist_prepend (copied, g_strdup (liter->data));
	copied = g_slist_reverse (copied);
	g_list_free (keys);

	for (siter = copied; siter; siter = g_slist_next (siter)) {
		gpointer value;

		value = g_hash_table_lookup (hash, siter->data);
		func (siter->data, value, user_data);
	}

	g_slist_foreach (copied, (GFunc) g_free, NULL);
	g_slist_free (copied);
}
