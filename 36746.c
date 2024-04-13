static int encode_setattr(struct xdr_stream *xdr, const struct nfs_setattrargs *arg, const struct nfs_server *server)
{
	int status;
	__be32 *p;
	
        RESERVE_SPACE(4+NFS4_STATEID_SIZE);
        WRITE32(OP_SETATTR);
	WRITEMEM(arg->stateid.data, NFS4_STATEID_SIZE);

        if ((status = encode_attrs(xdr, arg->iap, server)))
		return status;

        return 0;
}
