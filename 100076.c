static void tun_set_headroom(struct net_device *dev, int new_hr)
{
	struct tun_struct *tun = netdev_priv(dev);

	if (new_hr < NET_SKB_PAD)
		new_hr = NET_SKB_PAD;

	tun->align = new_hr;
}
