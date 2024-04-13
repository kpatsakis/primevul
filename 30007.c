NTSTATUS vfs_file_exist(connection_struct *conn, struct smb_filename *smb_fname)
{
	/* Only return OK if stat was successful and S_ISREG */
	if ((SMB_VFS_STAT(conn, smb_fname) != -1) &&
	    S_ISREG(smb_fname->st.st_ex_mode)) {
		return NT_STATUS_OK;
	}

	return NT_STATUS_OBJECT_NAME_NOT_FOUND;
}
