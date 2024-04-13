static void *nr_info_start(struct seq_file *seq, loff_t *pos)
{
	spin_lock_bh(&nr_list_lock);
	return seq_hlist_start_head(&nr_list, *pos);
}
