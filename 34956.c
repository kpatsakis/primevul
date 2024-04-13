static struct MHD_Response *create_response_file(const char *nurl,
						 const char *method,
						 unsigned int *rp_code,
						 const char *fpath)
{
	struct stat st;
	int ret;
	FILE *file;

	ret = stat(fpath, &st);

	if (!ret && (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode))) {
		file = fopen(fpath, "rb");

		if (file) {
			*rp_code = MHD_HTTP_OK;

			if (!st.st_size) {
				fclose(file);
				return MHD_create_response_from_data
					(0, NULL, MHD_NO, MHD_NO);
			}

			return MHD_create_response_from_callback
				(st.st_size,
				 32 * 1024,
				 &file_reader,
				 file,
				 (MHD_ContentReaderFreeCallback)&fclose);

		} else {
			log_err("Failed to open: %s.", fpath);
		}
	}

	return NULL;
}
