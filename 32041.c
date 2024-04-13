static void ssh2_pkt_defer_noqueue(Ssh ssh, struct Packet *pkt, int noignore)
{
    int len;
    if (ssh->cscipher != NULL && (ssh->cscipher->flags & SSH_CIPHER_IS_CBC) &&
	ssh->deferred_len == 0 && !noignore &&
	!(ssh->remote_bugs & BUG_CHOKES_ON_SSH2_IGNORE)) {
	/*
	 * Interpose an SSH_MSG_IGNORE to ensure that user data don't
	 * get encrypted with a known IV.
	 */
	struct Packet *ipkt = ssh2_pkt_init(SSH2_MSG_IGNORE);
	ssh2_pkt_addstring_start(ipkt);
	ssh2_pkt_defer_noqueue(ssh, ipkt, TRUE);
    }
    len = ssh2_pkt_construct(ssh, pkt);
    if (ssh->deferred_len + len > ssh->deferred_size) {
	ssh->deferred_size = ssh->deferred_len + len + 128;
	ssh->deferred_send_data = sresize(ssh->deferred_send_data,
					  ssh->deferred_size,
					  unsigned char);
    }
    memcpy(ssh->deferred_send_data + ssh->deferred_len, pkt->body, len);
    ssh->deferred_len += len;
    ssh->deferred_data_size += pkt->encrypted_len;
    ssh_free_packet(pkt);
}
