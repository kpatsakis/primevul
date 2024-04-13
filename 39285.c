server_unresponsive(struct TCP_Server_Info *server)
{
	/*
	 * We need to wait 2 echo intervals to make sure we handle such
	 * situations right:
	 * 1s  client sends a normal SMB request
	 * 2s  client gets a response
	 * 30s echo workqueue job pops, and decides we got a response recently
	 *     and don't need to send another
	 * ...
	 * 65s kernel_recvmsg times out, and we see that we haven't gotten
	 *     a response in >60s.
	 */
	if (server->tcpStatus == CifsGood &&
	    time_after(jiffies, server->lstrp + 2 * SMB_ECHO_INTERVAL)) {
		cifs_dbg(VFS, "Server %s has not responded in %d seconds. Reconnecting...\n",
			 server->hostname, (2 * SMB_ECHO_INTERVAL) / HZ);
		cifs_reconnect(server);
		wake_up(&server->response_q);
		return true;
	}

	return false;
}
