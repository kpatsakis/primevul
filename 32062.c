static void ssh_gotdata(Ssh ssh, const unsigned char *data, int datalen)
{
    /* Log raw data, if we're in that mode. */
    if (ssh->logctx)
	log_packet(ssh->logctx, PKT_INCOMING, -1, NULL, data, datalen,
		   0, NULL, NULL, 0, NULL);

    crBegin(ssh->ssh_gotdata_crstate);

    /*
     * To begin with, feed the characters one by one to the
     * protocol initialisation / selection function do_ssh_init().
     * When that returns 0, we're done with the initial greeting
     * exchange and can move on to packet discipline.
     */
    while (1) {
	int ret;		       /* need not be kept across crReturn */
	if (datalen == 0)
	    crReturnV;		       /* more data please */
	ret = ssh->do_ssh_init(ssh, *data);
	data++;
	datalen--;
	if (ret == 0)
	    break;
    }

    /*
     * We emerge from that loop when the initial negotiation is
     * over and we have selected an s_rdpkt function. Now pass
     * everything to s_rdpkt, and then pass the resulting packets
     * to the proper protocol handler.
     */

    while (1) {
	while (bufchain_size(&ssh->queued_incoming_data) > 0 || datalen > 0) {
	    if (ssh->frozen) {
		ssh_queue_incoming_data(ssh, &data, &datalen);
		/* This uses up all data and cannot cause anything interesting
		 * to happen; indeed, for anything to happen at all, we must
		 * return, so break out. */
		break;
	    } else if (bufchain_size(&ssh->queued_incoming_data) > 0) {
		/* This uses up some or all data, and may freeze the
		 * session. */
		ssh_process_queued_incoming_data(ssh);
	    } else {
		/* This uses up some or all data, and may freeze the
		 * session. */
		ssh_process_incoming_data(ssh, &data, &datalen);
	    }
	    /* FIXME this is probably EBW. */
	    if (ssh->state == SSH_STATE_CLOSED)
		return;
	}
	/* We're out of data. Go and get some more. */
	crReturnV;
    }
    crFinishV;
}
