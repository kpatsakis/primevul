static void ssh2_pkt_send_with_padding(Ssh ssh, struct Packet *pkt,
				       int padsize)
{
#if 0
    if (0) {
	/*
	 * The simplest way to do this is to adjust the
	 * variable-length padding field in the outgoing packet.
	 * 
	 * Currently compiled out, because some Cisco SSH servers
	 * don't like excessively padded packets (bah, why's it
	 * always Cisco?)
	 */
	pkt->forcepad = padsize;
	ssh2_pkt_send(ssh, pkt);
    } else
#endif
    {
	/*
	 * If we can't do that, however, an alternative approach is
	 * to use the pkt_defer mechanism to bundle the packet
	 * tightly together with an SSH_MSG_IGNORE such that their
	 * combined length is a constant. So first we construct the
	 * final form of this packet and defer its sending.
	 */
	ssh2_pkt_defer(ssh, pkt);

	/*
	 * Now construct an SSH_MSG_IGNORE which includes a string
	 * that's an exact multiple of the cipher block size. (If
	 * the cipher is NULL so that the block size is
	 * unavailable, we don't do this trick at all, because we
	 * gain nothing by it.)
	 */
	if (ssh->cscipher &&
	    !(ssh->remote_bugs & BUG_CHOKES_ON_SSH2_IGNORE)) {
	    int stringlen, i;

	    stringlen = (256 - ssh->deferred_len);
	    stringlen += ssh->cscipher->blksize - 1;
	    stringlen -= (stringlen % ssh->cscipher->blksize);
	    if (ssh->cscomp) {
		/*
		 * Temporarily disable actual compression, so we
		 * can guarantee to get this string exactly the
		 * length we want it. The compression-disabling
		 * routine should return an integer indicating how
		 * many bytes we should adjust our string length
		 * by.
		 */
		stringlen -=
		    ssh->cscomp->disable_compression(ssh->cs_comp_ctx);
	    }
	    pkt = ssh2_pkt_init(SSH2_MSG_IGNORE);
	    ssh2_pkt_addstring_start(pkt);
	    for (i = 0; i < stringlen; i++) {
		char c = (char) random_byte();
		ssh2_pkt_addstring_data(pkt, &c, 1);
	    }
	    ssh2_pkt_defer(ssh, pkt);
	}
	ssh_pkt_defersend(ssh);
    }
}
