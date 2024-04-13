static int encode_getattr_one(struct xdr_stream *xdr, uint32_t bitmap)
{
        __be32 *p;

        RESERVE_SPACE(12);
        WRITE32(OP_GETATTR);
        WRITE32(1);
        WRITE32(bitmap);
        return 0;
}
