cifs_mount(struct cifs_sb_info *cifs_sb, struct smb_vol *volume_info)
{
	int rc;
	unsigned int xid;
	struct cifs_ses *ses;
	struct cifs_tcon *tcon;
	struct TCP_Server_Info *server;
	char   *full_path;
	struct tcon_link *tlink;
#ifdef CONFIG_CIFS_DFS_UPCALL
	int referral_walks_count = 0;
#endif

	rc = bdi_setup_and_register(&cifs_sb->bdi, "cifs", BDI_CAP_MAP_COPY);
	if (rc)
		return rc;

#ifdef CONFIG_CIFS_DFS_UPCALL
try_mount_again:
	/* cleanup activities if we're chasing a referral */
	if (referral_walks_count) {
		if (tcon)
			cifs_put_tcon(tcon);
		else if (ses)
			cifs_put_smb_ses(ses);

		free_xid(xid);
	}
#endif
	rc = 0;
	tcon = NULL;
	ses = NULL;
	server = NULL;
	full_path = NULL;
	tlink = NULL;

	xid = get_xid();

	/* get a reference to a tcp session */
	server = cifs_get_tcp_session(volume_info);
	if (IS_ERR(server)) {
		rc = PTR_ERR(server);
		bdi_destroy(&cifs_sb->bdi);
		goto out;
	}

	/* get a reference to a SMB session */
	ses = cifs_get_smb_ses(server, volume_info);
	if (IS_ERR(ses)) {
		rc = PTR_ERR(ses);
		ses = NULL;
		goto mount_fail_check;
	}

	/* search for existing tcon to this server share */
	tcon = cifs_get_tcon(ses, volume_info);
	if (IS_ERR(tcon)) {
		rc = PTR_ERR(tcon);
		tcon = NULL;
		goto remote_path_check;
	}

	/* tell server which Unix caps we support */
	if (cap_unix(tcon->ses)) {
		/* reset of caps checks mount to see if unix extensions
		   disabled for just this mount */
		reset_cifs_unix_caps(xid, tcon, cifs_sb, volume_info);
		if ((tcon->ses->server->tcpStatus == CifsNeedReconnect) &&
		    (le64_to_cpu(tcon->fsUnixInfo.Capability) &
		     CIFS_UNIX_TRANSPORT_ENCRYPTION_MANDATORY_CAP)) {
			rc = -EACCES;
			goto mount_fail_check;
		}
	} else
		tcon->unix_ext = 0; /* server does not support them */

	/* do not care if a following call succeed - informational */
	if (!tcon->ipc && server->ops->qfs_tcon)
		server->ops->qfs_tcon(xid, tcon);

	cifs_sb->wsize = server->ops->negotiate_wsize(tcon, volume_info);
	cifs_sb->rsize = server->ops->negotiate_rsize(tcon, volume_info);

	/* tune readahead according to rsize */
	cifs_sb->bdi.ra_pages = cifs_sb->rsize / PAGE_CACHE_SIZE;

remote_path_check:
#ifdef CONFIG_CIFS_DFS_UPCALL
	/*
	 * Perform an unconditional check for whether there are DFS
	 * referrals for this path without prefix, to provide support
	 * for DFS referrals from w2k8 servers which don't seem to respond
	 * with PATH_NOT_COVERED to requests that include the prefix.
	 * Chase the referral if found, otherwise continue normally.
	 */
	if (referral_walks_count == 0) {
		int refrc = expand_dfs_referral(xid, ses, volume_info, cifs_sb,
						false);
		if (!refrc) {
			referral_walks_count++;
			goto try_mount_again;
		}
	}
#endif

	/* check if a whole path is not remote */
	if (!rc && tcon) {
		if (!server->ops->is_path_accessible) {
			rc = -ENOSYS;
			goto mount_fail_check;
		}
		/*
		 * cifs_build_path_to_root works only when we have a valid tcon
		 */
		full_path = cifs_build_path_to_root(volume_info, cifs_sb, tcon);
		if (full_path == NULL) {
			rc = -ENOMEM;
			goto mount_fail_check;
		}
		rc = server->ops->is_path_accessible(xid, tcon, cifs_sb,
						     full_path);
		if (rc != 0 && rc != -EREMOTE) {
			kfree(full_path);
			goto mount_fail_check;
		}
		kfree(full_path);
	}

	/* get referral if needed */
	if (rc == -EREMOTE) {
#ifdef CONFIG_CIFS_DFS_UPCALL
		if (referral_walks_count > MAX_NESTED_LINKS) {
			/*
			 * BB: when we implement proper loop detection,
			 *     we will remove this check. But now we need it
			 *     to prevent an indefinite loop if 'DFS tree' is
			 *     misconfigured (i.e. has loops).
			 */
			rc = -ELOOP;
			goto mount_fail_check;
		}

		rc = expand_dfs_referral(xid, ses, volume_info, cifs_sb, true);

		if (!rc) {
			referral_walks_count++;
			goto try_mount_again;
		}
		goto mount_fail_check;
#else /* No DFS support, return error on mount */
		rc = -EOPNOTSUPP;
#endif
	}

	if (rc)
		goto mount_fail_check;

	/* now, hang the tcon off of the superblock */
	tlink = kzalloc(sizeof *tlink, GFP_KERNEL);
	if (tlink == NULL) {
		rc = -ENOMEM;
		goto mount_fail_check;
	}

	tlink->tl_uid = ses->linux_uid;
	tlink->tl_tcon = tcon;
	tlink->tl_time = jiffies;
	set_bit(TCON_LINK_MASTER, &tlink->tl_flags);
	set_bit(TCON_LINK_IN_TREE, &tlink->tl_flags);

	cifs_sb->master_tlink = tlink;
	spin_lock(&cifs_sb->tlink_tree_lock);
	tlink_rb_insert(&cifs_sb->tlink_tree, tlink);
	spin_unlock(&cifs_sb->tlink_tree_lock);

	queue_delayed_work(cifsiod_wq, &cifs_sb->prune_tlinks,
				TLINK_IDLE_EXPIRE);

mount_fail_check:
	/* on error free sesinfo and tcon struct if needed */
	if (rc) {
		/* If find_unc succeeded then rc == 0 so we can not end */
		/* up accidentally freeing someone elses tcon struct */
		if (tcon)
			cifs_put_tcon(tcon);
		else if (ses)
			cifs_put_smb_ses(ses);
		else
			cifs_put_tcp_session(server);
		bdi_destroy(&cifs_sb->bdi);
	}

out:
	free_xid(xid);
	return rc;
}
