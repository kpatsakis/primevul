cifs_put_smb_ses(struct cifs_ses *ses)
{
	unsigned int xid;
	struct TCP_Server_Info *server = ses->server;

	cifs_dbg(FYI, "%s: ses_count=%d\n", __func__, ses->ses_count);
	spin_lock(&cifs_tcp_ses_lock);
	if (--ses->ses_count > 0) {
		spin_unlock(&cifs_tcp_ses_lock);
		return;
	}

	list_del_init(&ses->smb_ses_list);
	spin_unlock(&cifs_tcp_ses_lock);

	if (ses->status == CifsGood && server->ops->logoff) {
		xid = get_xid();
		server->ops->logoff(xid, ses);
		_free_xid(xid);
	}
	sesInfoFree(ses);
	cifs_put_tcp_session(server);
}
