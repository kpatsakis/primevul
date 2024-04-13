_zip_file_exists(zip_source_t *src, zip_error_t *error)
{
    struct zip_stat st;

    zip_stat_init(&st);
    if (zip_source_stat(src, &st) != 0) {
        zip_error_t *src_error = zip_source_error(src);
        if (zip_error_code_zip(src_error) == ZIP_ER_READ && zip_error_code_system(src_error) == ENOENT) {
	    return EXISTS_NOT;
	}
	_zip_error_copy(error, src_error);
	return EXISTS_ERROR;
    }

    return (st.valid & ZIP_STAT_SIZE) && st.size == 0 ? EXISTS_EMPTY : EXISTS_NONEMPTY;
}
