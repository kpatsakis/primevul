int ip_fragment(struct sk_buff *skb, int (*output)(struct sk_buff *))
{
	struct iphdr *iph;
	int ptr;
	struct net_device *dev;
	struct sk_buff *skb2;
	unsigned int mtu, hlen, left, len, ll_rs;
	int offset;
	__be16 not_last_frag;
	struct rtable *rt = skb_rtable(skb);
	int err = 0;

	dev = rt->dst.dev;

	/*
	 *	Point into the IP datagram header.
	 */

	iph = ip_hdr(skb);

	if (unlikely(((iph->frag_off & htons(IP_DF)) && !skb->local_df) ||
		     (IPCB(skb)->frag_max_size &&
		      IPCB(skb)->frag_max_size > dst_mtu(&rt->dst)))) {
		IP_INC_STATS(dev_net(dev), IPSTATS_MIB_FRAGFAILS);
		icmp_send(skb, ICMP_DEST_UNREACH, ICMP_FRAG_NEEDED,
			  htonl(ip_skb_dst_mtu(skb)));
		kfree_skb(skb);
		return -EMSGSIZE;
	}

	/*
	 *	Setup starting values.
	 */

	hlen = iph->ihl * 4;
	mtu = dst_mtu(&rt->dst) - hlen;	/* Size of data space */
#ifdef CONFIG_BRIDGE_NETFILTER
	if (skb->nf_bridge)
		mtu -= nf_bridge_mtu_reduction(skb);
#endif
	IPCB(skb)->flags |= IPSKB_FRAG_COMPLETE;

	/* When frag_list is given, use it. First, check its validity:
	 * some transformers could create wrong frag_list or break existing
	 * one, it is not prohibited. In this case fall back to copying.
	 *
	 * LATER: this step can be merged to real generation of fragments,
	 * we can switch to copy when see the first bad fragment.
	 */
	if (skb_has_frag_list(skb)) {
		struct sk_buff *frag, *frag2;
		int first_len = skb_pagelen(skb);

		if (first_len - hlen > mtu ||
		    ((first_len - hlen) & 7) ||
		    ip_is_fragment(iph) ||
		    skb_cloned(skb))
			goto slow_path;

		skb_walk_frags(skb, frag) {
			/* Correct geometry. */
			if (frag->len > mtu ||
			    ((frag->len & 7) && frag->next) ||
			    skb_headroom(frag) < hlen)
				goto slow_path_clean;

			/* Partially cloned skb? */
			if (skb_shared(frag))
				goto slow_path_clean;

			BUG_ON(frag->sk);
			if (skb->sk) {
				frag->sk = skb->sk;
				frag->destructor = sock_wfree;
			}
			skb->truesize -= frag->truesize;
		}

		/* Everything is OK. Generate! */

		err = 0;
		offset = 0;
		frag = skb_shinfo(skb)->frag_list;
		skb_frag_list_init(skb);
		skb->data_len = first_len - skb_headlen(skb);
		skb->len = first_len;
		iph->tot_len = htons(first_len);
		iph->frag_off = htons(IP_MF);
		ip_send_check(iph);

		for (;;) {
			/* Prepare header of the next frame,
			 * before previous one went down. */
			if (frag) {
				frag->ip_summed = CHECKSUM_NONE;
				skb_reset_transport_header(frag);
				__skb_push(frag, hlen);
				skb_reset_network_header(frag);
				memcpy(skb_network_header(frag), iph, hlen);
				iph = ip_hdr(frag);
				iph->tot_len = htons(frag->len);
				ip_copy_metadata(frag, skb);
				if (offset == 0)
					ip_options_fragment(frag);
				offset += skb->len - hlen;
				iph->frag_off = htons(offset>>3);
				if (frag->next != NULL)
					iph->frag_off |= htons(IP_MF);
				/* Ready, complete checksum */
				ip_send_check(iph);
			}

			err = output(skb);

			if (!err)
				IP_INC_STATS(dev_net(dev), IPSTATS_MIB_FRAGCREATES);
			if (err || !frag)
				break;

			skb = frag;
			frag = skb->next;
			skb->next = NULL;
		}

		if (err == 0) {
			IP_INC_STATS(dev_net(dev), IPSTATS_MIB_FRAGOKS);
			return 0;
		}

		while (frag) {
			skb = frag->next;
			kfree_skb(frag);
			frag = skb;
		}
		IP_INC_STATS(dev_net(dev), IPSTATS_MIB_FRAGFAILS);
		return err;

slow_path_clean:
		skb_walk_frags(skb, frag2) {
			if (frag2 == frag)
				break;
			frag2->sk = NULL;
			frag2->destructor = NULL;
			skb->truesize += frag2->truesize;
		}
	}

slow_path:
	/* for offloaded checksums cleanup checksum before fragmentation */
	if ((skb->ip_summed == CHECKSUM_PARTIAL) && skb_checksum_help(skb))
		goto fail;
	iph = ip_hdr(skb);

	left = skb->len - hlen;		/* Space per frame */
	ptr = hlen;		/* Where to start from */

	/* for bridged IP traffic encapsulated inside f.e. a vlan header,
	 * we need to make room for the encapsulating header
	 */
	ll_rs = LL_RESERVED_SPACE_EXTRA(rt->dst.dev, nf_bridge_pad(skb));

	/*
	 *	Fragment the datagram.
	 */

	offset = (ntohs(iph->frag_off) & IP_OFFSET) << 3;
	not_last_frag = iph->frag_off & htons(IP_MF);

	/*
	 *	Keep copying data until we run out.
	 */

	while (left > 0) {
		len = left;
		/* IF: it doesn't fit, use 'mtu' - the data space left */
		if (len > mtu)
			len = mtu;
		/* IF: we are not sending up to and including the packet end
		   then align the next start on an eight byte boundary */
		if (len < left)	{
			len &= ~7;
		}
		/*
		 *	Allocate buffer.
		 */

		if ((skb2 = alloc_skb(len+hlen+ll_rs, GFP_ATOMIC)) == NULL) {
			NETDEBUG(KERN_INFO "IP: frag: no memory for new fragment!\n");
			err = -ENOMEM;
			goto fail;
		}

		/*
		 *	Set up data on packet
		 */

		ip_copy_metadata(skb2, skb);
		skb_reserve(skb2, ll_rs);
		skb_put(skb2, len + hlen);
		skb_reset_network_header(skb2);
		skb2->transport_header = skb2->network_header + hlen;

		/*
		 *	Charge the memory for the fragment to any owner
		 *	it might possess
		 */

		if (skb->sk)
			skb_set_owner_w(skb2, skb->sk);

		/*
		 *	Copy the packet header into the new buffer.
		 */

		skb_copy_from_linear_data(skb, skb_network_header(skb2), hlen);

		/*
		 *	Copy a block of the IP datagram.
		 */
		if (skb_copy_bits(skb, ptr, skb_transport_header(skb2), len))
			BUG();
		left -= len;

		/*
		 *	Fill in the new header fields.
		 */
		iph = ip_hdr(skb2);
		iph->frag_off = htons((offset >> 3));

		/* ANK: dirty, but effective trick. Upgrade options only if
		 * the segment to be fragmented was THE FIRST (otherwise,
		 * options are already fixed) and make it ONCE
		 * on the initial skb, so that all the following fragments
		 * will inherit fixed options.
		 */
		if (offset == 0)
			ip_options_fragment(skb);

		/*
		 *	Added AC : If we are fragmenting a fragment that's not the
		 *		   last fragment then keep MF on each bit
		 */
		if (left > 0 || not_last_frag)
			iph->frag_off |= htons(IP_MF);
		ptr += len;
		offset += len;

		/*
		 *	Put this fragment into the sending queue.
		 */
		iph->tot_len = htons(len + hlen);

		ip_send_check(iph);

		err = output(skb2);
		if (err)
			goto fail;

		IP_INC_STATS(dev_net(dev), IPSTATS_MIB_FRAGCREATES);
	}
	consume_skb(skb);
	IP_INC_STATS(dev_net(dev), IPSTATS_MIB_FRAGOKS);
	return err;

fail:
	kfree_skb(skb);
	IP_INC_STATS(dev_net(dev), IPSTATS_MIB_FRAGFAILS);
	return err;
}
