mii_send_bits (struct net_device *dev, u32 data, int len)
{
	int i;
	for (i = len - 1; i >= 0; i--) {
		mii_sendbit (dev, data & (1 << i));
	}
}
