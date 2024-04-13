_zip_allocate_new(zip_source_t *src, unsigned int flags, zip_error_t *error)
{
    zip_t *za;

    if ((za = _zip_new(error)) == NULL) {
	return NULL;
    }

    za->src = src;
    za->open_flags = flags;
    if (flags & ZIP_RDONLY) {
        za->flags |= ZIP_AFL_RDONLY;
        za->ch_flags |= ZIP_AFL_RDONLY;
    }
    return za;
}
