static int encode_setclientid(struct xdr_stream *xdr, const struct nfs4_setclientid *setclientid)
{
	__be32 *p;

	RESERVE_SPACE(4 + NFS4_VERIFIER_SIZE);
	WRITE32(OP_SETCLIENTID);
	WRITEMEM(setclientid->sc_verifier->data, NFS4_VERIFIER_SIZE);

	encode_string(xdr, setclientid->sc_name_len, setclientid->sc_name);
	RESERVE_SPACE(4);
	WRITE32(setclientid->sc_prog);
	encode_string(xdr, setclientid->sc_netid_len, setclientid->sc_netid);
	encode_string(xdr, setclientid->sc_uaddr_len, setclientid->sc_uaddr);
	RESERVE_SPACE(4);
	WRITE32(setclientid->sc_cb_ident);

	return 0;
}
