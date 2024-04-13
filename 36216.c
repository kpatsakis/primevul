static int igmp_mc_seq_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &igmp_mc_seq_ops,
			sizeof(struct igmp_mc_iter_state));
}
