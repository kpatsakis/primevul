static int decode_lock_denied (struct xdr_stream *xdr, struct file_lock *fl)
{
	uint64_t offset, length, clientid;
	__be32 *p;
	uint32_t namelen, type;

	READ_BUF(32);
	READ64(offset);
	READ64(length);
	READ32(type);
	if (fl != NULL) {
		fl->fl_start = (loff_t)offset;
		fl->fl_end = fl->fl_start + (loff_t)length - 1;
		if (length == ~(uint64_t)0)
			fl->fl_end = OFFSET_MAX;
		fl->fl_type = F_WRLCK;
		if (type & 1)
			fl->fl_type = F_RDLCK;
		fl->fl_pid = 0;
	}
	READ64(clientid);
	READ32(namelen);
	READ_BUF(namelen);
	return -NFS4ERR_DENIED;
}
