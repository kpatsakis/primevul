cifs_async_writev(struct cifs_writedata *wdata)
{
	int i, rc = -EACCES;
	WRITE_REQ *smb = NULL;
	int wct;
	struct cifs_tcon *tcon = tlink_tcon(wdata->cfile->tlink);
	struct inode *inode = wdata->cfile->dentry->d_inode;
	struct kvec *iov = NULL;

	if (tcon->ses->capabilities & CAP_LARGE_FILES) {
		wct = 14;
	} else {
		wct = 12;
		if (wdata->offset >> 32 > 0) {
			/* can not handle big offset for old srv */
			return -EIO;
		}
	}

	rc = small_smb_init(SMB_COM_WRITE_ANDX, wct, tcon, (void **)&smb);
	if (rc)
		goto async_writev_out;

	/* 1 iov per page + 1 for header */
	iov = kzalloc((wdata->nr_pages + 1) * sizeof(*iov), GFP_NOFS);
	if (iov == NULL) {
		rc = -ENOMEM;
		goto async_writev_out;
	}

	smb->hdr.Pid = cpu_to_le16((__u16)wdata->cfile->pid);
	smb->hdr.PidHigh = cpu_to_le16((__u16)(wdata->cfile->pid >> 16));

	smb->AndXCommand = 0xFF;	/* none */
	smb->Fid = wdata->cfile->netfid;
	smb->OffsetLow = cpu_to_le32(wdata->offset & 0xFFFFFFFF);
	if (wct == 14)
		smb->OffsetHigh = cpu_to_le32(wdata->offset >> 32);
	smb->Reserved = 0xFFFFFFFF;
	smb->WriteMode = 0;
	smb->Remaining = 0;

	smb->DataOffset =
	    cpu_to_le16(offsetof(struct smb_com_write_req, Data) - 4);

	/* 4 for RFC1001 length + 1 for BCC */
	iov[0].iov_len = be32_to_cpu(smb->hdr.smb_buf_length) + 4 + 1;
	iov[0].iov_base = smb;

	/* marshal up the pages into iov array */
	wdata->bytes = 0;
	for (i = 0; i < wdata->nr_pages; i++) {
		iov[i + 1].iov_len = min(inode->i_size -
				      page_offset(wdata->pages[i]),
					(loff_t)PAGE_CACHE_SIZE);
		iov[i + 1].iov_base = kmap(wdata->pages[i]);
		wdata->bytes += iov[i + 1].iov_len;
	}

	cFYI(1, "async write at %llu %u bytes", wdata->offset, wdata->bytes);

	smb->DataLengthLow = cpu_to_le16(wdata->bytes & 0xFFFF);
	smb->DataLengthHigh = cpu_to_le16(wdata->bytes >> 16);

	if (wct == 14) {
		inc_rfc1001_len(&smb->hdr, wdata->bytes + 1);
		put_bcc(wdata->bytes + 1, &smb->hdr);
	} else {
		/* wct == 12 */
		struct smb_com_writex_req *smbw =
				(struct smb_com_writex_req *)smb;
		inc_rfc1001_len(&smbw->hdr, wdata->bytes + 5);
		put_bcc(wdata->bytes + 5, &smbw->hdr);
		iov[0].iov_len += 4; /* pad bigger by four bytes */
	}

	kref_get(&wdata->refcount);
	rc = cifs_call_async(tcon->ses->server, iov, wdata->nr_pages + 1,
			     cifs_writev_callback, wdata, false);

	if (rc == 0)
		cifs_stats_inc(&tcon->num_writes);
	else
		kref_put(&wdata->refcount, cifs_writedata_release);

	/* send is done, unmap pages */
	for (i = 0; i < wdata->nr_pages; i++)
		kunmap(wdata->pages[i]);

async_writev_out:
	cifs_small_buf_release(smb);
	kfree(iov);
	return rc;
}
