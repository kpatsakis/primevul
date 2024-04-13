get_dfs_path(const unsigned int xid, struct cifs_ses *ses, const char *old_path,
	     const struct nls_table *nls_codepage, unsigned int *num_referrals,
	     struct dfs_info3_param **referrals, int remap)
{
	char *temp_unc;
	int rc = 0;

	if (!ses->server->ops->tree_connect || !ses->server->ops->get_dfs_refer)
		return -ENOSYS;

	*num_referrals = 0;
	*referrals = NULL;

	if (ses->ipc_tid == 0) {
		temp_unc = kmalloc(2 /* for slashes */ +
			strnlen(ses->serverName, SERVER_NAME_LEN_WITH_NULL * 2)
				+ 1 + 4 /* slash IPC$ */ + 2, GFP_KERNEL);
		if (temp_unc == NULL)
			return -ENOMEM;
		temp_unc[0] = '\\';
		temp_unc[1] = '\\';
		strcpy(temp_unc + 2, ses->serverName);
		strcpy(temp_unc + 2 + strlen(ses->serverName), "\\IPC$");
		rc = ses->server->ops->tree_connect(xid, ses, temp_unc, NULL,
						    nls_codepage);
		cifs_dbg(FYI, "Tcon rc = %d ipc_tid = %d\n", rc, ses->ipc_tid);
		kfree(temp_unc);
	}
	if (rc == 0)
		rc = ses->server->ops->get_dfs_refer(xid, ses, old_path,
						     referrals, num_referrals,
						     nls_codepage, remap);
	/*
	 * BB - map targetUNCs to dfs_info3 structures, here or in
	 * ses->server->ops->get_dfs_refer.
	 */

	return rc;
}
