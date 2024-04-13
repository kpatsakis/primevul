const char *vfs_readdirname(connection_struct *conn, void *p,
			    SMB_STRUCT_STAT *sbuf, char **talloced)
{
	struct dirent *ptr= NULL;
	const char *dname;
	char *translated;
	NTSTATUS status;

	if (!p)
		return(NULL);

	ptr = SMB_VFS_READDIR(conn, (DIR *)p, sbuf);
	if (!ptr)
		return(NULL);

	dname = ptr->d_name;


#ifdef NEXT2
	if (telldir(p) < 0)
		return(NULL);
#endif

#ifdef HAVE_BROKEN_READDIR_NAME
	/* using /usr/ucb/cc is BAD */
	dname = dname - 2;
#endif

	status = SMB_VFS_TRANSLATE_NAME(conn, dname, vfs_translate_to_windows,
					talloc_tos(), &translated);
	if (NT_STATUS_EQUAL(status, NT_STATUS_NONE_MAPPED)) {
		*talloced = NULL;
		return dname;
	}
	*talloced = translated;
	if (!NT_STATUS_IS_OK(status)) {
		return NULL;
	}
	return translated;
}
