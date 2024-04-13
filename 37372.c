cifs_writev_complete(struct work_struct *work)
{
	struct cifs_writedata *wdata = container_of(work,
						struct cifs_writedata, work);
	struct inode *inode = wdata->cfile->dentry->d_inode;
	int i = 0;

	if (wdata->result == 0) {
		cifs_update_eof(CIFS_I(inode), wdata->offset, wdata->bytes);
		cifs_stats_bytes_written(tlink_tcon(wdata->cfile->tlink),
					 wdata->bytes);
	} else if (wdata->sync_mode == WB_SYNC_ALL && wdata->result == -EAGAIN)
		return cifs_writev_requeue(wdata);

	for (i = 0; i < wdata->nr_pages; i++) {
		struct page *page = wdata->pages[i];
		if (wdata->result == -EAGAIN)
			__set_page_dirty_nobuffers(page);
		else if (wdata->result < 0)
			SetPageError(page);
		end_page_writeback(page);
		page_cache_release(page);
	}
	if (wdata->result != -EAGAIN)
		mapping_set_error(inode->i_mapping, wdata->result);
	kref_put(&wdata->refcount, cifs_writedata_release);
}
