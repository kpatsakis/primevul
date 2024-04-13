_zip_read_cdir(zip_t *za, zip_buffer_t *buffer, zip_uint64_t buf_offset, zip_error_t *error)
{
    zip_cdir_t *cd;
    zip_uint16_t comment_len;
    zip_uint64_t i, left;
    zip_uint64_t eocd_offset = _zip_buffer_offset(buffer);
    zip_buffer_t *cd_buffer;

    if (_zip_buffer_left(buffer) < EOCDLEN) {
	/* not enough bytes left for comment */
	zip_error_set(error, ZIP_ER_NOZIP, 0);
	return NULL;
    }

    /* check for end-of-central-dir magic */
    if (memcmp(_zip_buffer_get(buffer, 4), EOCD_MAGIC, 4) != 0) {
	zip_error_set(error, ZIP_ER_NOZIP, 0);
	return NULL;
    }

    if (eocd_offset >= EOCD64LOCLEN && memcmp(_zip_buffer_data(buffer) + eocd_offset - EOCD64LOCLEN, EOCD64LOC_MAGIC, 4) == 0) {
        _zip_buffer_set_offset(buffer, eocd_offset - EOCD64LOCLEN);
        cd = _zip_read_eocd64(za->src, buffer, buf_offset, za->flags, error);
    }
    else {
        _zip_buffer_set_offset(buffer, eocd_offset);
        cd = _zip_read_eocd(buffer, buf_offset, za->flags, error);
    }

    if (cd == NULL)
	return NULL;

    _zip_buffer_set_offset(buffer, eocd_offset + 20);
    comment_len = _zip_buffer_get_16(buffer);

    if (cd->offset + cd->size > buf_offset + eocd_offset) {
	/* cdir spans past EOCD record */
	zip_error_set(error, ZIP_ER_INCONS, 0);
	_zip_cdir_free(cd);
	return NULL;
    }

    if (comment_len || (za->open_flags & ZIP_CHECKCONS)) {
        zip_uint64_t tail_len;

        _zip_buffer_set_offset(buffer, eocd_offset + EOCDLEN);
        tail_len = _zip_buffer_left(buffer);

        if (tail_len < comment_len || ((za->open_flags & ZIP_CHECKCONS) && tail_len != comment_len)) {
            zip_error_set(error, ZIP_ER_INCONS, 0);
            _zip_cdir_free(cd);
            return NULL;
        }

        if (comment_len) {
            if ((cd->comment=_zip_string_new(_zip_buffer_get(buffer, comment_len), comment_len, ZIP_FL_ENC_GUESS, error)) == NULL) {
                _zip_cdir_free(cd);
                return NULL;
            }
        }
    }

    if (cd->offset >= buf_offset) {
        zip_uint8_t *data;
	/* if buffer already read in, use it */
        _zip_buffer_set_offset(buffer, cd->offset - buf_offset);

        if ((data = _zip_buffer_get(buffer, cd->size)) == NULL) {
            zip_error_set(error, ZIP_ER_INCONS, 0);
            _zip_cdir_free(cd);
            return NULL;
        }
        if ((cd_buffer = _zip_buffer_new(data, cd->size)) == NULL) {
            zip_error_set(error, ZIP_ER_MEMORY, 0);
            _zip_cdir_free(cd);
            return NULL;
        }
    }
    else {
        cd_buffer = NULL;

        if (zip_source_seek(za->src, (zip_int64_t)cd->offset, SEEK_SET) < 0) {
            _zip_error_set_from_source(error, za->src);
            _zip_cdir_free(cd);
            return NULL;
        }

	/* possible consistency check: cd->offset = len-(cd->size+cd->comment_len+EOCDLEN) ? */
	if (zip_source_tell(za->src) != (zip_int64_t)cd->offset) {
            zip_error_set(error, ZIP_ER_NOZIP, 0);
	    _zip_cdir_free(cd);
	    return NULL;
	}
    }

    left = (zip_uint64_t)cd->size;
    i=0;
    while (left > 0) {
	bool grown = false;
        zip_int64_t entry_size;

	if (i == cd->nentry) {
	    /* InfoZIP has a hack to avoid using Zip64: it stores nentries % 0x10000 */
	    /* This hack isn't applicable if we're using Zip64, or if there is no central directory entry following. */

	    if (cd->is_zip64 || left < CDENTRYSIZE) {
		break;
	    }

	    if (!_zip_cdir_grow(cd, 0x10000, error)) {
		_zip_cdir_free(cd);
		_zip_buffer_free(cd_buffer);
		return NULL;
	    }
	    grown = true;
	}

	if ((cd->entry[i].orig=_zip_dirent_new()) == NULL || (entry_size = _zip_dirent_read(cd->entry[i].orig, za->src, cd_buffer, false, error)) < 0) {
	    if (grown && zip_error_code_zip(error) == ZIP_ER_NOZIP) {
		zip_error_set(error, ZIP_ER_INCONS, 0);
	    }
	    _zip_cdir_free(cd);
            _zip_buffer_free(cd_buffer);
	    return NULL;
	}
	i++;
        left -= (zip_uint64_t)entry_size;
    }

    if (i != cd->nentry || left > 0) {
        zip_error_set(error, ZIP_ER_INCONS, 0);
        _zip_buffer_free(cd_buffer);
        _zip_cdir_free(cd);
        return NULL;
    }

    if (za->open_flags & ZIP_CHECKCONS) {
        bool ok;

        if (cd_buffer) {
            ok = _zip_buffer_eof(cd_buffer);
        }
        else {
            zip_int64_t offset = zip_source_tell(za->src);

            if (offset < 0) {
                _zip_error_set_from_source(error, za->src);
                _zip_cdir_free(cd);
                return NULL;
            }
            ok = ((zip_uint64_t)offset == cd->offset + cd->size);
        }

        if (!ok) {
            zip_error_set(error, ZIP_ER_INCONS, 0);
            _zip_buffer_free(cd_buffer);
            _zip_cdir_free(cd);
            return NULL;
        }
    }

    _zip_buffer_free(cd_buffer);
    return cd;
}
