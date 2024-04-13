int phar_seek_efp(phar_entry_info *entry, off_t offset, int whence, off_t position, int follow_links TSRMLS_DC) /* {{{ */
{
	php_stream *fp = phar_get_efp(entry, follow_links TSRMLS_CC);
	off_t temp, eoffset;

	if (!fp) {
		return -1;
	}

	if (follow_links) {
		phar_entry_info *t;
		t = phar_get_link_source(entry TSRMLS_CC);
		if (t) {
			entry = t;
		}
	}

	if (entry->is_dir) {
		return 0;
	}

	eoffset = phar_get_fp_offset(entry TSRMLS_CC);

	switch (whence) {
		case SEEK_END:
			temp = eoffset + entry->uncompressed_filesize + offset;
			break;
		case SEEK_CUR:
			temp = eoffset + position + offset;
			break;
		case SEEK_SET:
			temp = eoffset + offset;
			break;
		default:
			temp = 0;
	}

	if (temp > eoffset + (off_t) entry->uncompressed_filesize) {
		return -1;
	}

	if (temp < eoffset) {
		return -1;
	}

	return php_stream_seek(fp, temp, SEEK_SET);
}
/* }}} */
