void *ping_seq_start(struct seq_file *seq, loff_t *pos, sa_family_t family)
{
	struct ping_iter_state *state = seq->private;
	state->bucket = 0;
	state->family = family;

	read_lock_bh(&ping_table.lock);

	return *pos ? ping_get_idx(seq, *pos-1) : SEQ_START_TOKEN;
}
