cifs_setup_session(const unsigned int xid, struct cifs_ses *ses,
		   struct nls_table *nls_info)
{
	int rc = -ENOSYS;
	struct TCP_Server_Info *server = ses->server;

	ses->flags = 0;
	ses->capabilities = server->capabilities;
	if (linuxExtEnabled == 0)
		ses->capabilities &= (~server->vals->cap_unix);

	cifs_dbg(FYI, "Security Mode: 0x%x Capabilities: 0x%x TimeAdjust: %d\n",
		 server->sec_mode, server->capabilities, server->timeAdj);

	if (server->ops->sess_setup)
		rc = server->ops->sess_setup(xid, ses, nls_info);

	if (rc) {
		cifs_dbg(VFS, "Send error in SessSetup = %d\n", rc);
	} else {
		mutex_lock(&server->srv_mutex);
		if (!server->session_estab) {
			server->session_key.response = ses->auth_key.response;
			server->session_key.len = ses->auth_key.len;
			server->sequence_number = 0x2;
			server->session_estab = true;
			ses->auth_key.response = NULL;
		}
		mutex_unlock(&server->srv_mutex);

		cifs_dbg(FYI, "CIFS Session Established successfully\n");
		spin_lock(&GlobalMid_Lock);
		ses->status = CifsGood;
		ses->need_reconnect = false;
		spin_unlock(&GlobalMid_Lock);
	}

	kfree(ses->auth_key.response);
	ses->auth_key.response = NULL;
	ses->auth_key.len = 0;
	kfree(ses->ntlmssp);
	ses->ntlmssp = NULL;

	return rc;
}
