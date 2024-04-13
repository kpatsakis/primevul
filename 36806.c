nfs4_xdr_enc_setacl(struct rpc_rqst *req, __be32 *p, struct nfs_setaclargs *args)
{
        struct xdr_stream xdr;
        struct compound_hdr hdr = {
                .nops   = 2,
        };
        int status;

        xdr_init_encode(&xdr, &req->rq_snd_buf, p);
        encode_compound_hdr(&xdr, &hdr);
        status = encode_putfh(&xdr, args->fh);
        if (status)
                goto out;
        status = encode_setacl(&xdr, args);
out:
        return status;
}
