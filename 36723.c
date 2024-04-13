static int encode_create(struct xdr_stream *xdr, const struct nfs4_create_arg *create)
{
	__be32 *p;
	
	RESERVE_SPACE(8);
	WRITE32(OP_CREATE);
	WRITE32(create->ftype);

	switch (create->ftype) {
	case NF4LNK:
		RESERVE_SPACE(4);
		WRITE32(create->u.symlink.len);
		xdr_write_pages(xdr, create->u.symlink.pages, 0, create->u.symlink.len);
		break;

	case NF4BLK: case NF4CHR:
		RESERVE_SPACE(8);
		WRITE32(create->u.device.specdata1);
		WRITE32(create->u.device.specdata2);
		break;

	default:
		break;
	}

	RESERVE_SPACE(4 + create->name->len);
	WRITE32(create->name->len);
	WRITEMEM(create->name->name, create->name->len);

	return encode_attrs(xdr, create->attrs, create->server);
}
