int cifs_mknod(struct inode *inode, struct dentry *direntry, int mode,
		dev_t device_number)
{
	int rc = -EPERM;
	int xid;
	int create_options = CREATE_NOT_DIR | CREATE_OPTION_SPECIAL;
	struct cifs_sb_info *cifs_sb;
	struct tcon_link *tlink;
	struct cifs_tcon *pTcon;
	struct cifs_io_parms io_parms;
	char *full_path = NULL;
	struct inode *newinode = NULL;
	int oplock = 0;
	u16 fileHandle;
	FILE_ALL_INFO *buf = NULL;
	unsigned int bytes_written;
	struct win_dev *pdev;

	if (!old_valid_dev(device_number))
		return -EINVAL;

	cifs_sb = CIFS_SB(inode->i_sb);
	tlink = cifs_sb_tlink(cifs_sb);
	if (IS_ERR(tlink))
		return PTR_ERR(tlink);

	pTcon = tlink_tcon(tlink);

	xid = GetXid();

	full_path = build_path_from_dentry(direntry);
	if (full_path == NULL) {
		rc = -ENOMEM;
		goto mknod_out;
	}

	if (pTcon->unix_ext) {
		struct cifs_unix_set_info_args args = {
			.mode	= mode & ~current_umask(),
			.ctime	= NO_CHANGE_64,
			.atime	= NO_CHANGE_64,
			.mtime	= NO_CHANGE_64,
			.device	= device_number,
		};
		if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_SET_UID) {
			args.uid = (__u64) current_fsuid();
			args.gid = (__u64) current_fsgid();
		} else {
			args.uid = NO_CHANGE_64;
			args.gid = NO_CHANGE_64;
		}
		rc = CIFSSMBUnixSetPathInfo(xid, pTcon, full_path, &args,
					    cifs_sb->local_nls,
					    cifs_sb->mnt_cifs_flags &
						CIFS_MOUNT_MAP_SPECIAL_CHR);
		if (rc)
			goto mknod_out;

		rc = cifs_get_inode_info_unix(&newinode, full_path,
						inode->i_sb, xid);

		if (rc == 0)
			d_instantiate(direntry, newinode);
		goto mknod_out;
	}

	if (!(cifs_sb->mnt_cifs_flags & CIFS_MOUNT_UNX_EMUL))
		goto mknod_out;


	cFYI(1, "sfu compat create special file");

	buf = kmalloc(sizeof(FILE_ALL_INFO), GFP_KERNEL);
	if (buf == NULL) {
		kfree(full_path);
		rc = -ENOMEM;
		FreeXid(xid);
		return rc;
	}

	if (backup_cred(cifs_sb))
		create_options |= CREATE_OPEN_BACKUP_INTENT;

	rc = CIFSSMBOpen(xid, pTcon, full_path, FILE_CREATE,
			 GENERIC_WRITE, create_options,
			 &fileHandle, &oplock, buf, cifs_sb->local_nls,
			 cifs_sb->mnt_cifs_flags & CIFS_MOUNT_MAP_SPECIAL_CHR);
	if (rc)
		goto mknod_out;

	/* BB Do not bother to decode buf since no local inode yet to put
	 * timestamps in, but we can reuse it safely */

	pdev = (struct win_dev *)buf;
	io_parms.netfid = fileHandle;
	io_parms.pid = current->tgid;
	io_parms.tcon = pTcon;
	io_parms.offset = 0;
	io_parms.length = sizeof(struct win_dev);
	if (S_ISCHR(mode)) {
		memcpy(pdev->type, "IntxCHR", 8);
		pdev->major =
		      cpu_to_le64(MAJOR(device_number));
		pdev->minor =
		      cpu_to_le64(MINOR(device_number));
		rc = CIFSSMBWrite(xid, &io_parms,
			&bytes_written, (char *)pdev,
			NULL, 0);
	} else if (S_ISBLK(mode)) {
		memcpy(pdev->type, "IntxBLK", 8);
		pdev->major =
		      cpu_to_le64(MAJOR(device_number));
		pdev->minor =
		      cpu_to_le64(MINOR(device_number));
		rc = CIFSSMBWrite(xid, &io_parms,
			&bytes_written, (char *)pdev,
			NULL, 0);
	} /* else if (S_ISFIFO) */
	CIFSSMBClose(xid, pTcon, fileHandle);
	d_drop(direntry);

	/* FIXME: add code here to set EAs */

mknod_out:
	kfree(full_path);
	kfree(buf);
	FreeXid(xid);
	cifs_put_tlink(tlink);
	return rc;
}
