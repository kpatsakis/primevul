void jbd2_journal_unfile_buffer(journal_t *journal, struct journal_head *jh)
{
	struct buffer_head *bh = jh2bh(jh);

	/* Get reference so that buffer cannot be freed before we unlock it */
	get_bh(bh);
	jbd_lock_bh_state(bh);
	spin_lock(&journal->j_list_lock);
	__jbd2_journal_unfile_buffer(jh);
	spin_unlock(&journal->j_list_lock);
	jbd_unlock_bh_state(bh);
	__brelse(bh);
}
