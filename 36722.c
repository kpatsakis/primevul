static int encode_commit(struct xdr_stream *xdr, const struct nfs_writeargs *args)
{
	__be32 *p;
        
        RESERVE_SPACE(16);
        WRITE32(OP_COMMIT);
        WRITE64(args->offset);
        WRITE32(args->count);

        return 0;
}
