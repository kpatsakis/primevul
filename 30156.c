update_secret_hash (NMSetting *setting,
                    GHashTable *secrets,
                    GError **error)
{
	NMSettingVPNPrivate *priv = NM_SETTING_VPN_GET_PRIVATE (setting);
	GHashTableIter iter;
	const char *name, *value;

	g_return_val_if_fail (secrets != NULL, FALSE);

	/* Make sure the items are valid */
	g_hash_table_iter_init (&iter, secrets);
	while (g_hash_table_iter_next (&iter, (gpointer *) &name, (gpointer *) &value)) {
		if (!name || !strlen (name)) {
			g_set_error_literal (error, NM_SETTING_ERROR,
			                     NM_SETTING_ERROR_PROPERTY_TYPE_MISMATCH,
			                     "Secret name was empty");
			return FALSE;
		}

		if (!value || !strlen (value)) {
			g_set_error (error, NM_SETTING_ERROR,
			             NM_SETTING_ERROR_PROPERTY_TYPE_MISMATCH,
				         "Secret %s value was empty", name);
			return FALSE;
		}
	}

	/* Now add the items to the settings' secrets list */
	g_hash_table_iter_init (&iter, secrets);
	while (g_hash_table_iter_next (&iter, (gpointer *) &name, (gpointer *) &value)) {
		if (value == NULL) {
			g_warn_if_fail (value != NULL);
			continue;
		}
		if (strlen (value) == 0) {
			g_warn_if_fail (strlen (value) > 0);
			continue;
		}

		g_hash_table_insert (priv->secrets, g_strdup (name), g_strdup (value));
	}

	return TRUE;
}
