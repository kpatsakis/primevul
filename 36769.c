static int nfs4_xdr_dec_open_noattr(struct rpc_rqst *rqstp, __be32 *p, struct nfs_openres *res)
{
        struct xdr_stream xdr;
        struct compound_hdr hdr;
        int status;

        xdr_init_decode(&xdr, &rqstp->rq_rcv_buf, p);
        status = decode_compound_hdr(&xdr, &hdr);
        if (status)
                goto out;
        status = decode_putfh(&xdr);
        if (status)
                goto out;
        status = decode_open(&xdr, res);
        if (status)
                goto out;
	decode_getfattr(&xdr, res->f_attr, res->server);
out:
        return status;
}
