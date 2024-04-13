_zip_read_eocd(zip_buffer_t *buffer, zip_uint64_t buf_offset, unsigned int flags, zip_error_t *error)
{
    zip_cdir_t *cd;
    zip_uint64_t i, nentry, size, offset, eocd_offset;

    if (_zip_buffer_left(buffer) < EOCDLEN) {
	zip_error_set(error, ZIP_ER_INCONS, 0);
	return NULL;
    }

    eocd_offset = _zip_buffer_offset(buffer);

    _zip_buffer_get(buffer, 4); /* magic already verified */

    if (_zip_buffer_get_32(buffer) != 0) {
	zip_error_set(error, ZIP_ER_MULTIDISK, 0);
	return NULL;
    }

    /* number of cdir-entries on this disk */
    i = _zip_buffer_get_16(buffer);
    /* number of cdir-entries */
    nentry = _zip_buffer_get_16(buffer);

    if (nentry != i) {
	zip_error_set(error, ZIP_ER_NOZIP, 0);
	return NULL;
    }

    size = _zip_buffer_get_32(buffer);
    offset = _zip_buffer_get_32(buffer);

    if (offset+size < offset) {
        zip_error_set(error, ZIP_ER_SEEK, EFBIG);
        return NULL;
    }

    if (offset+size > buf_offset + eocd_offset) {
	/* cdir spans past EOCD record */
	zip_error_set(error, ZIP_ER_INCONS, 0);
	return NULL;
    }

    if ((flags & ZIP_CHECKCONS) && offset+size != buf_offset + eocd_offset) {
	zip_error_set(error, ZIP_ER_INCONS, 0);
	return NULL;
    }

    if ((cd=_zip_cdir_new(nentry, error)) == NULL)
	return NULL;

    cd->is_zip64 = false;
    cd->size = size;
    cd->offset = offset;

    return cd;
}
