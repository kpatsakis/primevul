static void s_wrpkt_defer(Ssh ssh, struct Packet *pkt)
{
    int len, offset;
    len = s_wrpkt_prepare(ssh, pkt, &offset);
    if (ssh->deferred_len + len > ssh->deferred_size) {
	ssh->deferred_size = ssh->deferred_len + len + 128;
	ssh->deferred_send_data = sresize(ssh->deferred_send_data,
					  ssh->deferred_size,
					  unsigned char);
    }
    memcpy(ssh->deferred_send_data + ssh->deferred_len,
	   pkt->data + offset, len);
    ssh->deferred_len += len;
    ssh_free_packet(pkt);
}
