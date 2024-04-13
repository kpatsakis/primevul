static int encode_putrootfh(struct xdr_stream *xdr)
{
        __be32 *p;
        
        RESERVE_SPACE(4);
        WRITE32(OP_PUTROOTFH);

        return 0;
}
