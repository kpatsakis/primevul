cifs_put_tcon(struct cifsTconInfo *tcon)
{
	int xid;
	struct cifsSesInfo *ses = tcon->ses;

	cFYI(1, "%s: tc_count=%d\n", __func__, tcon->tc_count);
	spin_lock(&cifs_tcp_ses_lock);
	if (--tcon->tc_count > 0) {
		spin_unlock(&cifs_tcp_ses_lock);
		return;
	}

	list_del_init(&tcon->tcon_list);
	spin_unlock(&cifs_tcp_ses_lock);

	xid = GetXid();
	CIFSSMBTDis(xid, tcon);
	_FreeXid(xid);

	cifs_fscache_release_super_cookie(tcon);
	tconInfoFree(tcon);
	cifs_put_smb_ses(ses);
}
