cifs_writev_requeue(struct cifs_writedata *wdata)
{
	int i, rc;
	struct inode *inode = wdata->cfile->dentry->d_inode;

	for (i = 0; i < wdata->nr_pages; i++) {
		lock_page(wdata->pages[i]);
		clear_page_dirty_for_io(wdata->pages[i]);
	}

	do {
		rc = cifs_async_writev(wdata);
	} while (rc == -EAGAIN);

	for (i = 0; i < wdata->nr_pages; i++) {
		if (rc != 0)
			SetPageError(wdata->pages[i]);
		unlock_page(wdata->pages[i]);
	}

	mapping_set_error(inode->i_mapping, rc);
	kref_put(&wdata->refcount, cifs_writedata_release);
}
