static int igmp_mcf_seq_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &igmp_mcf_seq_ops,
			sizeof(struct igmp_mcf_iter_state));
}
