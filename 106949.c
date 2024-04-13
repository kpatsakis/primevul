zip_open(const char *fn, int _flags, int *zep)
{
    zip_t *za;
    zip_source_t *src;
    struct zip_error error;

    zip_error_init(&error);
    if ((src = zip_source_file_create(fn, 0, -1, &error)) == NULL) {
	_zip_set_open_error(zep, &error, 0);
	zip_error_fini(&error);
	return NULL;
    }

    if ((za = zip_open_from_source(src, _flags, &error)) == NULL) {
	zip_source_free(src);
	_zip_set_open_error(zep, &error, 0);
	zip_error_fini(&error);
	return NULL;
    }

    zip_error_fini(&error);
    return za;
}
