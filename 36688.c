static int decode_delegation(struct xdr_stream *xdr, struct nfs_openres *res)
{
        __be32 *p;
        uint32_t delegation_type;

	READ_BUF(4);
	READ32(delegation_type);
	if (delegation_type == NFS4_OPEN_DELEGATE_NONE) {
		res->delegation_type = 0;
		return 0;
	}
	READ_BUF(NFS4_STATEID_SIZE+4);
	COPYMEM(res->delegation.data, NFS4_STATEID_SIZE);
	READ32(res->do_recall);
	switch (delegation_type) {
		case NFS4_OPEN_DELEGATE_READ:
			res->delegation_type = FMODE_READ;
			break;
		case NFS4_OPEN_DELEGATE_WRITE:
			res->delegation_type = FMODE_WRITE|FMODE_READ;
			if (decode_space_limit(xdr, &res->maxsize) < 0)
				return -EIO;
	}
	return decode_ace(xdr, NULL, res->server->nfs_client);
}
