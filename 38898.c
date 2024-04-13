static int ip_vs_stats_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, ip_vs_stats_show, NULL);
}
