static int __net_init igmp_net_init(struct net *net)
{
	struct proc_dir_entry *pde;

	pde = proc_net_fops_create(net, "igmp", S_IRUGO, &igmp_mc_seq_fops);
	if (!pde)
		goto out_igmp;
	pde = proc_net_fops_create(net, "mcfilter", S_IRUGO, &igmp_mcf_seq_fops);
	if (!pde)
		goto out_mcfilter;
	return 0;

out_mcfilter:
	proc_net_remove(net, "igmp");
out_igmp:
	return -ENOMEM;
}
