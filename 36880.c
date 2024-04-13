cifs_echo_request(struct work_struct *work)
{
	int rc;
	struct TCP_Server_Info *server = container_of(work,
					struct TCP_Server_Info, echo.work);

	/*
	 * We cannot send an echo until the NEGOTIATE_PROTOCOL request is
	 * done, which is indicated by maxBuf != 0. Also, no need to ping if
	 * we got a response recently
	 */
	if (server->maxBuf == 0 ||
	    time_before(jiffies, server->lstrp + SMB_ECHO_INTERVAL - HZ))
		goto requeue_echo;

	rc = CIFSSMBEcho(server);
	if (rc)
		cFYI(1, "Unable to send echo request to server: %s",
			server->hostname);

requeue_echo:
	queue_delayed_work(system_nrt_wq, &server->echo, SMB_ECHO_INTERVAL);
}
