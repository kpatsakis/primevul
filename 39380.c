static int br_mdb_add(struct sk_buff *skb, struct nlmsghdr *nlh)
{
	struct net *net = sock_net(skb->sk);
	struct br_mdb_entry *entry;
	struct net_device *dev;
	struct net_bridge *br;
	int err;

	err = br_mdb_parse(skb, nlh, &dev, &entry);
	if (err < 0)
		return err;

	br = netdev_priv(dev);

	err = __br_mdb_add(net, br, entry);
	if (!err)
		__br_mdb_notify(dev, entry, RTM_NEWMDB);
	return err;
}
