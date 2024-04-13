static int decode_getfattr(struct xdr_stream *xdr, struct nfs_fattr *fattr, const struct nfs_server *server)
{
	__be32 *savep;
	uint32_t attrlen,
		 bitmap[2] = {0},
		 type;
	int status, fmode = 0;
	uint64_t fileid;

	if ((status = decode_op_hdr(xdr, OP_GETATTR)) != 0)
		goto xdr_error;
	if ((status = decode_attr_bitmap(xdr, bitmap)) != 0)
		goto xdr_error;

	fattr->bitmap[0] = bitmap[0];
	fattr->bitmap[1] = bitmap[1];

	if ((status = decode_attr_length(xdr, &attrlen, &savep)) != 0)
		goto xdr_error;


	if ((status = decode_attr_type(xdr, bitmap, &type)) != 0)
		goto xdr_error;
	fattr->type = nfs_type2fmt[type].nfs2type;
	fmode = nfs_type2fmt[type].mode;

	if ((status = decode_attr_change(xdr, bitmap, &fattr->change_attr)) != 0)
		goto xdr_error;
	if ((status = decode_attr_size(xdr, bitmap, &fattr->size)) != 0)
		goto xdr_error;
	if ((status = decode_attr_fsid(xdr, bitmap, &fattr->fsid)) != 0)
		goto xdr_error;
	if ((status = decode_attr_fileid(xdr, bitmap, &fattr->fileid)) != 0)
		goto xdr_error;
	if ((status = decode_attr_fs_locations(xdr, bitmap, container_of(fattr,
						struct nfs4_fs_locations,
						fattr))) != 0)
		goto xdr_error;
	if ((status = decode_attr_mode(xdr, bitmap, &fattr->mode)) != 0)
		goto xdr_error;
	fattr->mode |= fmode;
	if ((status = decode_attr_nlink(xdr, bitmap, &fattr->nlink)) != 0)
		goto xdr_error;
	if ((status = decode_attr_owner(xdr, bitmap, server->nfs_client, &fattr->uid)) != 0)
		goto xdr_error;
	if ((status = decode_attr_group(xdr, bitmap, server->nfs_client, &fattr->gid)) != 0)
		goto xdr_error;
	if ((status = decode_attr_rdev(xdr, bitmap, &fattr->rdev)) != 0)
		goto xdr_error;
	if ((status = decode_attr_space_used(xdr, bitmap, &fattr->du.nfs3.used)) != 0)
		goto xdr_error;
	if ((status = decode_attr_time_access(xdr, bitmap, &fattr->atime)) != 0)
		goto xdr_error;
	if ((status = decode_attr_time_metadata(xdr, bitmap, &fattr->ctime)) != 0)
		goto xdr_error;
	if ((status = decode_attr_time_modify(xdr, bitmap, &fattr->mtime)) != 0)
		goto xdr_error;
	if ((status = decode_attr_mounted_on_fileid(xdr, bitmap, &fileid)) != 0)
		goto xdr_error;
	if (fattr->fileid == 0 && fileid != 0)
		fattr->fileid = fileid;
	if ((status = verify_attr_len(xdr, savep, attrlen)) == 0)
		fattr->valid = NFS_ATTR_FATTR | NFS_ATTR_FATTR_V3 | NFS_ATTR_FATTR_V4;
xdr_error:
	dprintk("%s: xdr returned %d\n", __func__, -status);
	return status;
}
