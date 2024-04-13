cifs_reconnect(struct TCP_Server_Info *server)
{
	int rc = 0;
	struct list_head *tmp, *tmp2;
	struct cifsSesInfo *ses;
	struct cifsTconInfo *tcon;
	struct mid_q_entry *mid_entry;

	spin_lock(&GlobalMid_Lock);
	if (server->tcpStatus == CifsExiting) {
		/* the demux thread will exit normally
		next time through the loop */
		spin_unlock(&GlobalMid_Lock);
		return rc;
	} else
		server->tcpStatus = CifsNeedReconnect;
	spin_unlock(&GlobalMid_Lock);
	server->maxBuf = 0;

	cFYI(1, "Reconnecting tcp session");

	/* before reconnecting the tcp session, mark the smb session (uid)
		and the tid bad so they are not used until reconnected */
	cFYI(1, "%s: marking sessions and tcons for reconnect", __func__);
	spin_lock(&cifs_tcp_ses_lock);
	list_for_each(tmp, &server->smb_ses_list) {
		ses = list_entry(tmp, struct cifsSesInfo, smb_ses_list);
		ses->need_reconnect = true;
		ses->ipc_tid = 0;
		list_for_each(tmp2, &ses->tcon_list) {
			tcon = list_entry(tmp2, struct cifsTconInfo, tcon_list);
			tcon->need_reconnect = true;
		}
	}
	spin_unlock(&cifs_tcp_ses_lock);

	/* do not want to be sending data on a socket we are freeing */
	cFYI(1, "%s: tearing down socket", __func__);
	mutex_lock(&server->srv_mutex);
	if (server->ssocket) {
		cFYI(1, "State: 0x%x Flags: 0x%lx", server->ssocket->state,
			server->ssocket->flags);
		kernel_sock_shutdown(server->ssocket, SHUT_WR);
		cFYI(1, "Post shutdown state: 0x%x Flags: 0x%lx",
			server->ssocket->state,
			server->ssocket->flags);
		sock_release(server->ssocket);
		server->ssocket = NULL;
	}
	server->sequence_number = 0;
	server->session_estab = false;
	kfree(server->session_key.response);
	server->session_key.response = NULL;
	server->session_key.len = 0;
	server->lstrp = jiffies;
	mutex_unlock(&server->srv_mutex);

	/* mark submitted MIDs for retry and issue callback */
	cFYI(1, "%s: issuing mid callbacks", __func__);
	spin_lock(&GlobalMid_Lock);
	list_for_each_safe(tmp, tmp2, &server->pending_mid_q) {
		mid_entry = list_entry(tmp, struct mid_q_entry, qhead);
		if (mid_entry->midState == MID_REQUEST_SUBMITTED)
			mid_entry->midState = MID_RETRY_NEEDED;
		list_del_init(&mid_entry->qhead);
		mid_entry->callback(mid_entry);
	}
	spin_unlock(&GlobalMid_Lock);

	while ((server->tcpStatus != CifsExiting) &&
	       (server->tcpStatus != CifsGood)) {
		try_to_freeze();

		/* we should try only the port we connected to before */
		rc = generic_ip_connect(server);
		if (rc) {
			cFYI(1, "reconnect error %d", rc);
			msleep(3000);
		} else {
			atomic_inc(&tcpSesReconnectCount);
			spin_lock(&GlobalMid_Lock);
			if (server->tcpStatus != CifsExiting)
				server->tcpStatus = CifsGood;
			spin_unlock(&GlobalMid_Lock);
		}
	}

	return rc;
}
