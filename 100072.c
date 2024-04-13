static ssize_t tun_put_user(struct tun_struct *tun,
			    struct tun_file *tfile,
			    struct sk_buff *skb,
			    struct iov_iter *iter)
{
	struct tun_pi pi = { 0, skb->protocol };
	struct tun_pcpu_stats *stats;
	ssize_t total;
	int vlan_offset = 0;
	int vlan_hlen = 0;
	int vnet_hdr_sz = 0;

	if (skb_vlan_tag_present(skb))
		vlan_hlen = VLAN_HLEN;

	if (tun->flags & IFF_VNET_HDR)
		vnet_hdr_sz = READ_ONCE(tun->vnet_hdr_sz);

	total = skb->len + vlan_hlen + vnet_hdr_sz;

	if (!(tun->flags & IFF_NO_PI)) {
		if (iov_iter_count(iter) < sizeof(pi))
			return -EINVAL;

		total += sizeof(pi);
		if (iov_iter_count(iter) < total) {
			/* Packet will be striped */
			pi.flags |= TUN_PKT_STRIP;
		}

		if (copy_to_iter(&pi, sizeof(pi), iter) != sizeof(pi))
			return -EFAULT;
	}

	if (vnet_hdr_sz) {
		struct virtio_net_hdr gso;

		if (iov_iter_count(iter) < vnet_hdr_sz)
			return -EINVAL;

		if (virtio_net_hdr_from_skb(skb, &gso,
					    tun_is_little_endian(tun), true)) {
			struct skb_shared_info *sinfo = skb_shinfo(skb);
			pr_err("unexpected GSO type: "
			       "0x%x, gso_size %d, hdr_len %d\n",
			       sinfo->gso_type, tun16_to_cpu(tun, gso.gso_size),
			       tun16_to_cpu(tun, gso.hdr_len));
			print_hex_dump(KERN_ERR, "tun: ",
				       DUMP_PREFIX_NONE,
				       16, 1, skb->head,
				       min((int)tun16_to_cpu(tun, gso.hdr_len), 64), true);
			WARN_ON_ONCE(1);
			return -EINVAL;
		}

		if (copy_to_iter(&gso, sizeof(gso), iter) != sizeof(gso))
			return -EFAULT;

		iov_iter_advance(iter, vnet_hdr_sz - sizeof(gso));
	}

	if (vlan_hlen) {
		int ret;
		struct {
			__be16 h_vlan_proto;
			__be16 h_vlan_TCI;
		} veth;

		veth.h_vlan_proto = skb->vlan_proto;
		veth.h_vlan_TCI = htons(skb_vlan_tag_get(skb));

		vlan_offset = offsetof(struct vlan_ethhdr, h_vlan_proto);

		ret = skb_copy_datagram_iter(skb, 0, iter, vlan_offset);
		if (ret || !iov_iter_count(iter))
			goto done;

		ret = copy_to_iter(&veth, sizeof(veth), iter);
		if (ret != sizeof(veth) || !iov_iter_count(iter))
			goto done;
	}

	skb_copy_datagram_iter(skb, vlan_offset, iter, skb->len - vlan_offset);

done:
	/* caller is in process context, */
	stats = get_cpu_ptr(tun->pcpu_stats);
	u64_stats_update_begin(&stats->syncp);
	stats->tx_packets++;
	stats->tx_bytes += skb->len + vlan_hlen;
	u64_stats_update_end(&stats->syncp);
	put_cpu_ptr(tun->pcpu_stats);

	return total;
}
