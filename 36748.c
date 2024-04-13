static int encode_setclientid_confirm(struct xdr_stream *xdr, const struct nfs_client *client_state)
{
        __be32 *p;

        RESERVE_SPACE(12 + NFS4_VERIFIER_SIZE);
        WRITE32(OP_SETCLIENTID_CONFIRM);
        WRITE64(client_state->cl_clientid);
        WRITEMEM(client_state->cl_confirm.data, NFS4_VERIFIER_SIZE);

        return 0;
}
