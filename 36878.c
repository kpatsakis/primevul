cifs_construct_tcon(struct cifs_sb_info *cifs_sb, uid_t fsuid)
{
	struct cifsTconInfo *master_tcon = cifs_sb_master_tcon(cifs_sb);
	struct cifsSesInfo *ses;
	struct cifsTconInfo *tcon = NULL;
	struct smb_vol *vol_info;
	char username[MAX_USERNAME_SIZE + 1];

	vol_info = kzalloc(sizeof(*vol_info), GFP_KERNEL);
	if (vol_info == NULL) {
		tcon = ERR_PTR(-ENOMEM);
		goto out;
	}

	snprintf(username, MAX_USERNAME_SIZE, "krb50x%x", fsuid);
	vol_info->username = username;
	vol_info->local_nls = cifs_sb->local_nls;
	vol_info->linux_uid = fsuid;
	vol_info->cred_uid = fsuid;
	vol_info->UNC = master_tcon->treeName;
	vol_info->retry = master_tcon->retry;
	vol_info->nocase = master_tcon->nocase;
	vol_info->local_lease = master_tcon->local_lease;
	vol_info->no_linux_ext = !master_tcon->unix_ext;

	/* FIXME: allow for other secFlg settings */
	vol_info->secFlg = CIFSSEC_MUST_KRB5;

	/* get a reference for the same TCP session */
	spin_lock(&cifs_tcp_ses_lock);
	++master_tcon->ses->server->srv_count;
	spin_unlock(&cifs_tcp_ses_lock);

	ses = cifs_get_smb_ses(master_tcon->ses->server, vol_info);
	if (IS_ERR(ses)) {
		tcon = (struct cifsTconInfo *)ses;
		cifs_put_tcp_session(master_tcon->ses->server);
		goto out;
	}

	tcon = cifs_get_tcon(ses, vol_info);
	if (IS_ERR(tcon)) {
		cifs_put_smb_ses(ses);
		goto out;
	}

	if (ses->capabilities & CAP_UNIX)
		reset_cifs_unix_caps(0, tcon, NULL, vol_info);
out:
	kfree(vol_info);

	return tcon;
}
