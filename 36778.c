static int nfs4_xdr_dec_setattr(struct rpc_rqst *rqstp, __be32 *p, struct nfs_setattrres *res)
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
        status = decode_setattr(&xdr, res);
        if (status)
                goto out;
	status = decode_getfattr(&xdr, res->fattr, res->server);
	if (status == NFS4ERR_DELAY)
		status = 0;
out:
        return status;
}
