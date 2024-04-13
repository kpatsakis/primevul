int jbd2_journal_force_commit(journal_t *journal)
{
	handle_t *handle;
	int ret;

	handle = jbd2_journal_start(journal, 1);
	if (IS_ERR(handle)) {
		ret = PTR_ERR(handle);
	} else {
		handle->h_sync = 1;
		ret = jbd2_journal_stop(handle);
	}
	return ret;
}
