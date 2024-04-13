static bool tun_can_build_skb(struct tun_struct *tun, struct tun_file *tfile,
			      int len, int noblock, bool zerocopy)
{
	if ((tun->flags & TUN_TYPE_MASK) != IFF_TAP)
		return false;

	if (tfile->socket.sk->sk_sndbuf != INT_MAX)
		return false;

	if (!noblock)
		return false;

	if (zerocopy)
		return false;

	if (SKB_DATA_ALIGN(len + TUN_RX_PAD) +
	    SKB_DATA_ALIGN(sizeof(struct skb_shared_info)) > PAGE_SIZE)
		return false;

	return true;
}
