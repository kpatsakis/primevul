_zip_headercomp(const zip_dirent_t *central, const zip_dirent_t *local)
{
    if ((central->version_needed < local->version_needed)
#if 0
	/* some zip-files have different values in local
	   and global headers for the bitflags */
	|| (central->bitflags != local->bitflags)
#endif
	|| (central->comp_method != local->comp_method)
	|| (central->last_mod != local->last_mod)
	|| !_zip_string_equal(central->filename, local->filename))
	return -1;

    if ((central->crc != local->crc) || (central->comp_size != local->comp_size)
	|| (central->uncomp_size != local->uncomp_size)) {
	/* InfoZip stores valid values in local header even when data descriptor is used.
	   This is in violation of the appnote. */
	if (((local->bitflags & ZIP_GPBF_DATA_DESCRIPTOR) == 0
	     || local->crc != 0 || local->comp_size != 0 || local->uncomp_size != 0))
	    return -1;
    }

    return 0;
}
