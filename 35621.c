static void nfs4_construct_boot_verifier(struct nfs_client *clp,
					 nfs4_verifier *bootverf)
{
	__be32 verf[2];

	verf[0] = htonl((u32)clp->cl_boot_time.tv_sec);
	verf[1] = htonl((u32)clp->cl_boot_time.tv_nsec);
	memcpy(bootverf->data, verf, sizeof(bootverf->data));
}
