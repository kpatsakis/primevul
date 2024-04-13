cifs_writedata_alloc(unsigned int nr_pages)
{
	struct cifs_writedata *wdata;

	/* this would overflow */
	if (nr_pages == 0) {
		cERROR(1, "%s: called with nr_pages == 0!", __func__);
		return NULL;
	}

	/* writedata + number of page pointers */
	wdata = kzalloc(sizeof(*wdata) +
			sizeof(struct page *) * (nr_pages - 1), GFP_NOFS);
	if (wdata != NULL) {
		INIT_WORK(&wdata->work, cifs_writev_complete);
		kref_init(&wdata->refcount);
	}
	return wdata;
}
