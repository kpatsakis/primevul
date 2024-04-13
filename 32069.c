static void ssh_pkt_defersend(Ssh ssh)
{
    int backlog;
    backlog = s_write(ssh, ssh->deferred_send_data, ssh->deferred_len);
    ssh->deferred_len = ssh->deferred_size = 0;
    sfree(ssh->deferred_send_data);
    ssh->deferred_send_data = NULL;
    if (backlog > SSH_MAX_BACKLOG)
	ssh_throttle_all(ssh, 1, backlog);

    if (ssh->version == 2) {
	ssh->outgoing_data_size += ssh->deferred_data_size;
	ssh->deferred_data_size = 0;
	if (!ssh->kex_in_progress &&
	    !ssh->bare_connection &&
	    ssh->max_data_size != 0 &&
	    ssh->outgoing_data_size > ssh->max_data_size)
	    do_ssh2_transport(ssh, "too much data sent", -1, NULL);
    }
}
