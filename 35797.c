rio_tx_timeout (struct net_device *dev)
{
	long ioaddr = dev->base_addr;

	printk (KERN_INFO "%s: Tx timed out (%4.4x), is buffer full?\n",
		dev->name, readl (ioaddr + TxStatus));
	rio_free_tx(dev, 0);
	dev->if_port = 0;
	dev->trans_start = jiffies; /* prevent tx timeout */
}
