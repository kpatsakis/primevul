static void __exit nr_exit(void)
{
	int i;

	remove_proc_entry("nr", init_net.proc_net);
	remove_proc_entry("nr_neigh", init_net.proc_net);
	remove_proc_entry("nr_nodes", init_net.proc_net);
	nr_loopback_clear();

	nr_rt_free();

#ifdef CONFIG_SYSCTL
	nr_unregister_sysctl();
#endif

	ax25_linkfail_release(&nr_linkfail_notifier);
	ax25_protocol_release(AX25_P_NETROM);

	unregister_netdevice_notifier(&nr_dev_notifier);

	sock_unregister(PF_NETROM);

	for (i = 0; i < nr_ndevs; i++) {
		struct net_device *dev = dev_nr[i];
		if (dev) {
			unregister_netdev(dev);
			free_netdev(dev);
		}
	}

	kfree(dev_nr);
	proto_unregister(&nr_proto);
}
