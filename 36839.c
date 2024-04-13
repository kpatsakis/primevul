handle_t *jbd2__journal_start(journal_t *journal, int nblocks, gfp_t gfp_mask)
{
	handle_t *handle = journal_current_handle();
	int err;

	if (!journal)
		return ERR_PTR(-EROFS);

	if (handle) {
		J_ASSERT(handle->h_transaction->t_journal == journal);
		handle->h_ref++;
		return handle;
	}

	handle = new_handle(nblocks);
	if (!handle)
		return ERR_PTR(-ENOMEM);

	current->journal_info = handle;

	err = start_this_handle(journal, handle, gfp_mask);
	if (err < 0) {
		jbd2_free_handle(handle);
		current->journal_info = NULL;
		handle = ERR_PTR(err);
	}
	return handle;
}
