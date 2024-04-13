_zip_checkcons(zip_t *za, zip_cdir_t *cd, zip_error_t *error)
{
    zip_uint64_t i;
    zip_uint64_t min, max, j;
    struct zip_dirent temp;

    _zip_dirent_init(&temp);
    if (cd->nentry) {
	max = cd->entry[0].orig->offset;
	min = cd->entry[0].orig->offset;
    }
    else
	min = max = 0;

    for (i=0; i<cd->nentry; i++) {
	if (cd->entry[i].orig->offset < min)
	    min = cd->entry[i].orig->offset;
	if (min > (zip_uint64_t)cd->offset) {
	    zip_error_set(error, ZIP_ER_NOZIP, 0);
	    return -1;
	}

	j = cd->entry[i].orig->offset + cd->entry[i].orig->comp_size
	    + _zip_string_length(cd->entry[i].orig->filename) + LENTRYSIZE;
	if (j > max)
	    max = j;
	if (max > (zip_uint64_t)cd->offset) {
	    zip_error_set(error, ZIP_ER_NOZIP, 0);
	    return -1;
	}

        if (zip_source_seek(za->src, (zip_int64_t)cd->entry[i].orig->offset, SEEK_SET) < 0) {
            _zip_error_set_from_source(error, za->src);
            return -1;
	}

	if (_zip_dirent_read(&temp, za->src, NULL, true, error) == -1) {
	    _zip_dirent_finalize(&temp);
	    return -1;
	}

	if (_zip_headercomp(cd->entry[i].orig, &temp) != 0) {
	    zip_error_set(error, ZIP_ER_INCONS, 0);
	    _zip_dirent_finalize(&temp);
	    return -1;
	}

	cd->entry[i].orig->extra_fields = _zip_ef_merge(cd->entry[i].orig->extra_fields, temp.extra_fields);
	cd->entry[i].orig->local_extra_fields_read = 1;
	temp.extra_fields = NULL;

	_zip_dirent_finalize(&temp);
    }

    return (max-min) < ZIP_INT64_MAX ? (zip_int64_t)(max-min) : ZIP_INT64_MAX;
}
