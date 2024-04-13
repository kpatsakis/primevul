int vfs_ChDir(connection_struct *conn, const char *path)
{
	int ret;

	if (!LastDir) {
		LastDir = SMB_STRDUP("");
	}

	if (ISDOT(path)) {
		return 0;
	}

	if (*path == '/' && strcsequal(LastDir,path)) {
		return 0;
	}

	DEBUG(4,("vfs_ChDir to %s\n",path));

	ret = SMB_VFS_CHDIR(conn,path);
	if (ret == 0) {
		/* Global cache. */
		SAFE_FREE(LastDir);
		LastDir = SMB_STRDUP(path);

		/* conn cache. */
		TALLOC_FREE(conn->cwd);
		conn->cwd = vfs_GetWd(conn, conn);
		DEBUG(4,("vfs_ChDir got %s\n",conn->cwd));
	}
	return ret;
}
