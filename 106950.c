zip_open_from_source(zip_source_t *src, int _flags, zip_error_t *error)
{
    static zip_int64_t needed_support_read = -1;
    static zip_int64_t needed_support_write = -1;

    unsigned int flags;
    zip_int64_t supported;
    exists_t exists;

    if (_flags < 0 || src == NULL) {
	zip_error_set(error, ZIP_ER_INVAL, 0);
        return NULL;
    }
    flags = (unsigned int)_flags;

    supported = zip_source_supports(src);
    if (needed_support_read == -1) {
        needed_support_read = zip_source_make_command_bitmap(ZIP_SOURCE_OPEN, ZIP_SOURCE_READ, ZIP_SOURCE_CLOSE, ZIP_SOURCE_SEEK, ZIP_SOURCE_TELL, ZIP_SOURCE_STAT, -1);
        needed_support_write = zip_source_make_command_bitmap(ZIP_SOURCE_BEGIN_WRITE, ZIP_SOURCE_COMMIT_WRITE, ZIP_SOURCE_ROLLBACK_WRITE, ZIP_SOURCE_SEEK_WRITE, ZIP_SOURCE_TELL_WRITE, ZIP_SOURCE_REMOVE, -1);
    }
    if ((supported & needed_support_read) != needed_support_read) {
        zip_error_set(error, ZIP_ER_OPNOTSUPP, 0);
        return NULL;
    }
    if ((supported & needed_support_write) != needed_support_write) {
        flags |= ZIP_RDONLY;
    }

    if ((flags & (ZIP_RDONLY|ZIP_TRUNCATE)) == (ZIP_RDONLY|ZIP_TRUNCATE)) {
	zip_error_set(error, ZIP_ER_RDONLY, 0);
	return NULL;
    }

    exists = _zip_file_exists(src, error);
    switch (exists) {
    case EXISTS_ERROR:
	return NULL;

    case EXISTS_NOT:
	if ((flags & ZIP_CREATE) == 0) {
	    zip_error_set(error, ZIP_ER_NOENT, 0);
	    return NULL;
	}
	return _zip_allocate_new(src, flags, error);

    default: {
	zip_t *za;
	if (flags & ZIP_EXCL) {
	    zip_error_set(error, ZIP_ER_EXISTS, 0);
	    return NULL;
	}
	if (zip_source_open(src) < 0) {
	    _zip_error_set_from_source(error, src);
	    return NULL;
	}

	if (flags & ZIP_TRUNCATE) {
	    za = _zip_allocate_new(src, flags, error);
	}
	else {
	    /* ZIP_CREATE gets ignored if file exists and not ZIP_EXCL, just like open() */
	    za = _zip_open(src, flags, error);
	}

	if (za == NULL) {
	    zip_source_close(src);
	    return NULL;
	}
	return za;
    }
    }
}
