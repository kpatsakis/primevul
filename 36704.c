static int decode_pathname(struct xdr_stream *xdr, struct nfs4_pathname *path)
{
	u32 n;
	__be32 *p;
	int status = 0;

	READ_BUF(4);
	READ32(n);
	if (n == 0)
		goto root_path;
	dprintk("path ");
	path->ncomponents = 0;
	while (path->ncomponents < n) {
		struct nfs4_string *component = &path->components[path->ncomponents];
		status = decode_opaque_inline(xdr, &component->len, &component->data);
		if (unlikely(status != 0))
			goto out_eio;
		if (path->ncomponents != n)
			dprintk("/");
		dprintk("%s", component->data);
		if (path->ncomponents < NFS4_PATHNAME_MAXCOMPONENTS)
			path->ncomponents++;
		else {
			dprintk("cannot parse %d components in path\n", n);
			goto out_eio;
		}
	}
out:
	dprintk("\n");
	return status;
root_path:
/* a root pathname is sent as a zero component4 */
	path->ncomponents = 1;
	path->components[0].len=0;
	path->components[0].data=NULL;
	dprintk("path /\n");
	goto out;
out_eio:
	dprintk(" status %d", status);
	status = -EIO;
	goto out;
}
