allocate_buffers(struct TCP_Server_Info *server)
{
	if (!server->bigbuf) {
		server->bigbuf = (char *)cifs_buf_get();
		if (!server->bigbuf) {
			cifs_dbg(VFS, "No memory for large SMB response\n");
			msleep(3000);
			/* retry will check if exiting */
			return false;
		}
	} else if (server->large_buf) {
		/* we are reusing a dirty large buf, clear its start */
		memset(server->bigbuf, 0, HEADER_SIZE(server));
	}

	if (!server->smallbuf) {
		server->smallbuf = (char *)cifs_small_buf_get();
		if (!server->smallbuf) {
			cifs_dbg(VFS, "No memory for SMB response\n");
			msleep(1000);
			/* retry will check if exiting */
			return false;
		}
		/* beginning of smb buffer is cleared in our buf_get */
	} else {
		/* if existing small buf clear beginning */
		memset(server->smallbuf, 0, HEADER_SIZE(server));
	}

	return true;
}
