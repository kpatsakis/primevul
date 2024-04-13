static void ssh2_pkt_send_noqueue(Ssh ssh, struct Packet *pkt)
{
    int len;
    int backlog;
    if (ssh->cscipher != NULL && (ssh->cscipher->flags & SSH_CIPHER_IS_CBC)) {
	/* We need to send two packets, so use the deferral mechanism. */
	ssh2_pkt_defer_noqueue(ssh, pkt, FALSE);
	ssh_pkt_defersend(ssh);
	return;
    }
    len = ssh2_pkt_construct(ssh, pkt);
    backlog = s_write(ssh, pkt->body, len);
    if (backlog > SSH_MAX_BACKLOG)
	ssh_throttle_all(ssh, 1, backlog);

    ssh->outgoing_data_size += pkt->encrypted_len;
    if (!ssh->kex_in_progress &&
        !ssh->bare_connection &&
	ssh->max_data_size != 0 &&
	ssh->outgoing_data_size > ssh->max_data_size)
	do_ssh2_transport(ssh, "too much data sent", -1, NULL);

    ssh_free_packet(pkt);
}
