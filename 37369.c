cifs_reconnect_tcon(struct cifs_tcon *tcon, int smb_command)
{
	int rc;
	struct cifs_ses *ses;
	struct TCP_Server_Info *server;
	struct nls_table *nls_codepage;

	/*
	 * SMBs NegProt, SessSetup, uLogoff do not have tcon yet so check for
	 * tcp and smb session status done differently for those three - in the
	 * calling routine
	 */
	if (!tcon)
		return 0;

	ses = tcon->ses;
	server = ses->server;

	/*
	 * only tree disconnect, open, and write, (and ulogoff which does not
	 * have tcon) are allowed as we start force umount
	 */
	if (tcon->tidStatus == CifsExiting) {
		if (smb_command != SMB_COM_WRITE_ANDX &&
		    smb_command != SMB_COM_OPEN_ANDX &&
		    smb_command != SMB_COM_TREE_DISCONNECT) {
			cFYI(1, "can not send cmd %d while umounting",
				smb_command);
			return -ENODEV;
		}
	}

	/*
	 * Give demultiplex thread up to 10 seconds to reconnect, should be
	 * greater than cifs socket timeout which is 7 seconds
	 */
	while (server->tcpStatus == CifsNeedReconnect) {
		wait_event_interruptible_timeout(server->response_q,
			(server->tcpStatus != CifsNeedReconnect), 10 * HZ);

		/* are we still trying to reconnect? */
		if (server->tcpStatus != CifsNeedReconnect)
			break;

		/*
		 * on "soft" mounts we wait once. Hard mounts keep
		 * retrying until process is killed or server comes
		 * back on-line
		 */
		if (!tcon->retry) {
			cFYI(1, "gave up waiting on reconnect in smb_init");
			return -EHOSTDOWN;
		}
	}

	if (!ses->need_reconnect && !tcon->need_reconnect)
		return 0;

	nls_codepage = load_nls_default();

	/*
	 * need to prevent multiple threads trying to simultaneously
	 * reconnect the same SMB session
	 */
	mutex_lock(&ses->session_mutex);
	rc = cifs_negotiate_protocol(0, ses);
	if (rc == 0 && ses->need_reconnect)
		rc = cifs_setup_session(0, ses, nls_codepage);

	/* do we need to reconnect tcon? */
	if (rc || !tcon->need_reconnect) {
		mutex_unlock(&ses->session_mutex);
		goto out;
	}

	mark_open_files_invalid(tcon);
	rc = CIFSTCon(0, ses, tcon->treeName, tcon, nls_codepage);
	mutex_unlock(&ses->session_mutex);
	cFYI(1, "reconnect tcon rc = %d", rc);

	if (rc)
		goto out;

	/*
	 * FIXME: check if wsize needs updated due to negotiated smb buffer
	 * 	  size shrinking
	 */
	atomic_inc(&tconInfoReconnectCount);

	/* tell server Unix caps we support */
	if (ses->capabilities & CAP_UNIX)
		reset_cifs_unix_caps(0, tcon, NULL, NULL);

	/*
	 * Removed call to reopen open files here. It is safer (and faster) to
	 * reopen files one at a time as needed in read and write.
	 *
	 * FIXME: what about file locks? don't we need to reclaim them ASAP?
	 */

out:
	/*
	 * Check if handle based operation so we know whether we can continue
	 * or not without returning to caller to reset file handle
	 */
	switch (smb_command) {
	case SMB_COM_READ_ANDX:
	case SMB_COM_WRITE_ANDX:
	case SMB_COM_CLOSE:
	case SMB_COM_FIND_CLOSE2:
	case SMB_COM_LOCKING_ANDX:
		rc = -EAGAIN;
	}

	unload_nls(nls_codepage);
	return rc;
}
