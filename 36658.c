static int decode_attr_fs_locations(struct xdr_stream *xdr, uint32_t *bitmap, struct nfs4_fs_locations *res)
{
	int n;
	__be32 *p;
	int status = -EIO;

	if (unlikely(bitmap[0] & (FATTR4_WORD0_FS_LOCATIONS -1U)))
		goto out;
	status = 0;
	if (unlikely(!(bitmap[0] & FATTR4_WORD0_FS_LOCATIONS)))
		goto out;
	dprintk("%s: fsroot ", __func__);
	status = decode_pathname(xdr, &res->fs_path);
	if (unlikely(status != 0))
		goto out;
	READ_BUF(4);
	READ32(n);
	if (n <= 0)
		goto out_eio;
	res->nlocations = 0;
	while (res->nlocations < n) {
		u32 m;
		struct nfs4_fs_location *loc = &res->locations[res->nlocations];

		READ_BUF(4);
		READ32(m);

		loc->nservers = 0;
		dprintk("%s: servers ", __func__);
		while (loc->nservers < m) {
			struct nfs4_string *server = &loc->servers[loc->nservers];
			status = decode_opaque_inline(xdr, &server->len, &server->data);
			if (unlikely(status != 0))
				goto out_eio;
			dprintk("%s ", server->data);
			if (loc->nservers < NFS4_FS_LOCATION_MAXSERVERS)
				loc->nservers++;
			else {
				unsigned int i;
				dprintk("%s: using first %u of %u servers "
					"returned for location %u\n",
						__func__,
						NFS4_FS_LOCATION_MAXSERVERS,
						m, res->nlocations);
				for (i = loc->nservers; i < m; i++) {
					unsigned int len;
					char *data;
					status = decode_opaque_inline(xdr, &len, &data);
					if (unlikely(status != 0))
						goto out_eio;
				}
			}
		}
		status = decode_pathname(xdr, &loc->rootpath);
		if (unlikely(status != 0))
			goto out_eio;
		if (res->nlocations < NFS4_FS_LOCATIONS_MAXENTRIES)
			res->nlocations++;
	}
out:
	dprintk("%s: fs_locations done, error = %d\n", __func__, status);
	return status;
out_eio:
	status = -EIO;
	goto out;
}
