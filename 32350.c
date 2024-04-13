int tun_mainloop(struct openconnect_info *vpninfo, int *timeout)
{
	int work_done = 0;
	int prefix_size = 0;

#ifdef TUN_HAS_AF_PREFIX
	if (!vpninfo->script_tun)
		prefix_size = sizeof(int);
#endif

	if (FD_ISSET(vpninfo->tun_fd, &vpninfo->select_rfds)) {
		while (1) {
			int len = vpninfo->mtu;

			if (!out_pkt) {
				out_pkt = malloc(sizeof(struct pkt) + len);
				if (!out_pkt) {
					vpn_progress(vpninfo, PRG_ERR, "Allocation failed\n");
					break;
				}
			}

			len = read(vpninfo->tun_fd, out_pkt->data - prefix_size, len + prefix_size);
			if (len <= prefix_size)
				break;
			out_pkt->len = len - prefix_size;

			queue_packet(&vpninfo->outgoing_queue, out_pkt);
			out_pkt = NULL;

			work_done = 1;
			vpninfo->outgoing_qlen++;
			if (vpninfo->outgoing_qlen == vpninfo->max_qlen) {
				FD_CLR(vpninfo->tun_fd, &vpninfo->select_rfds);
				break;
			}
		}
	} else if (vpninfo->outgoing_qlen < vpninfo->max_qlen) {
		FD_SET(vpninfo->tun_fd, &vpninfo->select_rfds);
	}

	/* The kernel returns -ENOMEM when the queue is full, so theoretically
	   we could handle that and retry... but it doesn't let us poll() for
	   the no-longer-full situation, so let's not bother. */
	while (vpninfo->incoming_queue) {
		struct pkt *this = vpninfo->incoming_queue;
		unsigned char *data = this->data;
		int len = this->len;

#ifdef TUN_HAS_AF_PREFIX
		if (!vpninfo->script_tun) {
			struct ip *iph = (void *)data;
			int type;

			if (iph->ip_v == 6)
				type = AF_INET6;
			else if (iph->ip_v == 4)
				type = AF_INET;
			else {
				static int complained = 0;
				if (!complained) {
					complained = 1;
					vpn_progress(vpninfo, PRG_ERR,
						     _("Unknown packet (len %d) received: %02x %02x %02x %02x...\n"),
						     len, data[0], data[1], data[2], data[3]);
				}
				free(this);
				continue;
			}
			data -= 4;
			len += 4;
			*(int *)data = htonl(type);
		}
#endif
		vpninfo->incoming_queue = this->next;

		if (write(vpninfo->tun_fd, data, len) < 0) {
			/* Handle death of "script" socket */
			if (vpninfo->script_tun && errno == ENOTCONN) {
				vpninfo->quit_reason = "Client connection terminated";
				return 1;
			}
			vpn_progress(vpninfo, PRG_ERR,
				     _("Failed to write incoming packet: %s\n"),
				     strerror(errno));
		}
		free(this);
	}
	/* Work is not done if we just got rid of packets off the queue */
	return work_done;
}
