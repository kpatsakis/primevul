cifs_find_tcon(struct cifsSesInfo *ses, const char *unc)
{
	struct list_head *tmp;
	struct cifsTconInfo *tcon;

	spin_lock(&cifs_tcp_ses_lock);
	list_for_each(tmp, &ses->tcon_list) {
		tcon = list_entry(tmp, struct cifsTconInfo, tcon_list);
		if (tcon->tidStatus == CifsExiting)
			continue;
		if (strncmp(tcon->treeName, unc, MAX_TREE_SIZE))
			continue;

		++tcon->tc_count;
		spin_unlock(&cifs_tcp_ses_lock);
		return tcon;
	}
	spin_unlock(&cifs_tcp_ses_lock);
	return NULL;
}
