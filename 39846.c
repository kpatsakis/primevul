static int nr_sendmsg(struct kiocb *iocb, struct socket *sock,
		      struct msghdr *msg, size_t len)
{
	struct sock *sk = sock->sk;
	struct nr_sock *nr = nr_sk(sk);
	struct sockaddr_ax25 *usax = (struct sockaddr_ax25 *)msg->msg_name;
	int err;
	struct sockaddr_ax25 sax;
	struct sk_buff *skb;
	unsigned char *asmptr;
	int size;

	if (msg->msg_flags & ~(MSG_DONTWAIT|MSG_EOR|MSG_CMSG_COMPAT))
		return -EINVAL;

	lock_sock(sk);
	if (sock_flag(sk, SOCK_ZAPPED)) {
		err = -EADDRNOTAVAIL;
		goto out;
	}

	if (sk->sk_shutdown & SEND_SHUTDOWN) {
		send_sig(SIGPIPE, current, 0);
		err = -EPIPE;
		goto out;
	}

	if (nr->device == NULL) {
		err = -ENETUNREACH;
		goto out;
	}

	if (usax) {
		if (msg->msg_namelen < sizeof(sax)) {
			err = -EINVAL;
			goto out;
		}
		sax = *usax;
		if (ax25cmp(&nr->dest_addr, &sax.sax25_call) != 0) {
			err = -EISCONN;
			goto out;
		}
		if (sax.sax25_family != AF_NETROM) {
			err = -EINVAL;
			goto out;
		}
	} else {
		if (sk->sk_state != TCP_ESTABLISHED) {
			err = -ENOTCONN;
			goto out;
		}
		sax.sax25_family = AF_NETROM;
		sax.sax25_call   = nr->dest_addr;
	}

	/* Build a packet - the conventional user limit is 236 bytes. We can
	   do ludicrously large NetROM frames but must not overflow */
	if (len > 65536) {
		err = -EMSGSIZE;
		goto out;
	}

	size = len + NR_NETWORK_LEN + NR_TRANSPORT_LEN;

	if ((skb = sock_alloc_send_skb(sk, size, msg->msg_flags & MSG_DONTWAIT, &err)) == NULL)
		goto out;

	skb_reserve(skb, size - len);
	skb_reset_transport_header(skb);

	/*
	 *	Push down the NET/ROM header
	 */

	asmptr = skb_push(skb, NR_TRANSPORT_LEN);

	/* Build a NET/ROM Transport header */

	*asmptr++ = nr->your_index;
	*asmptr++ = nr->your_id;
	*asmptr++ = 0;		/* To be filled in later */
	*asmptr++ = 0;		/*      Ditto            */
	*asmptr++ = NR_INFO;

	/*
	 *	Put the data on the end
	 */
	skb_put(skb, len);

	/* User data follows immediately after the NET/ROM transport header */
	if (memcpy_fromiovec(skb_transport_header(skb), msg->msg_iov, len)) {
		kfree_skb(skb);
		err = -EFAULT;
		goto out;
	}

	if (sk->sk_state != TCP_ESTABLISHED) {
		kfree_skb(skb);
		err = -ENOTCONN;
		goto out;
	}

	nr_output(sk, skb);	/* Shove it onto the queue */

	err = len;
out:
	release_sock(sk);
	return err;
}
