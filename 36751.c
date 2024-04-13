__be32 *nfs4_decode_dirent(__be32 *p, struct nfs_entry *entry, int plus)
{
	uint32_t bitmap[2] = {0};
	uint32_t len;

	if (!*p++) {
		if (!*p)
			return ERR_PTR(-EAGAIN);
		entry->eof = 1;
		return ERR_PTR(-EBADCOOKIE);
	}

	entry->prev_cookie = entry->cookie;
	p = xdr_decode_hyper(p, &entry->cookie);
	entry->len = ntohl(*p++);
	entry->name = (const char *) p;
	p += XDR_QUADLEN(entry->len);

	/*
	 * In case the server doesn't return an inode number,
	 * we fake one here.  (We don't use inode number 0,
	 * since glibc seems to choke on it...)
	 */
	entry->ino = 1;

	len = ntohl(*p++);		/* bitmap length */
	if (len-- > 0) {
		bitmap[0] = ntohl(*p++);
		if (len-- > 0) {
			bitmap[1] = ntohl(*p++);
			p += len;
		}
	}
	len = XDR_QUADLEN(ntohl(*p++));	/* attribute buffer length */
	if (len > 0) {
		if (bitmap[0] & FATTR4_WORD0_RDATTR_ERROR) {
			bitmap[0] &= ~FATTR4_WORD0_RDATTR_ERROR;
			/* Ignore the return value of rdattr_error for now */
			p++;
			len--;
		}
		if (bitmap[0] == 0 && bitmap[1] == FATTR4_WORD1_MOUNTED_ON_FILEID)
			xdr_decode_hyper(p, &entry->ino);
		else if (bitmap[0] == FATTR4_WORD0_FILEID)
			xdr_decode_hyper(p, &entry->ino);
		p += len;
	}

	entry->eof = !p[0] && p[1];
	return p;
}
