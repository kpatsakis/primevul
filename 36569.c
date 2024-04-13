int nfs4_map_errors(int err)
{
	if (err < -1000) {
		dprintk("%s could not handle NFSv4 error %d\n",
				__func__, -err);
		return -EIO;
	}
	return err;
}
