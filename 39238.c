cifs_cleanup_volume_info(struct smb_vol *volume_info)
{
	if (!volume_info)
		return;
	cleanup_volume_info_contents(volume_info);
	kfree(volume_info);
}
