static void tun_queue_purge(struct tun_file *tfile)
{
	struct sk_buff *skb;

	while ((skb = skb_array_consume(&tfile->tx_array)) != NULL)
		kfree_skb(skb);

	skb_queue_purge(&tfile->sk.sk_write_queue);
	skb_queue_purge(&tfile->sk.sk_error_queue);
}
