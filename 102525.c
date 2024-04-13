file_trycdf(struct magic_set *ms, int fd, const unsigned char *buf,
    size_t nbytes)
{
        cdf_info_t info;
        cdf_header_t h;
        cdf_sat_t sat, ssat;
        cdf_stream_t sst, scn;
        cdf_dir_t dir;
        int i;
        const char *expn = "";
        const char *corrupt = "corrupt: ";

        info.i_fd = fd;
        info.i_buf = buf;
        info.i_len = nbytes;
        if (ms->flags & MAGIC_APPLE)
                return 0;
        if (cdf_read_header(&info, &h) == -1)
                return 0;
#ifdef CDF_DEBUG
        cdf_dump_header(&h);
#endif

        if ((i = cdf_read_sat(&info, &h, &sat)) == -1) {
                expn = "Can't read SAT";
                goto out0;
        }
#ifdef CDF_DEBUG
        cdf_dump_sat("SAT", &sat, CDF_SEC_SIZE(&h));
#endif

        if ((i = cdf_read_ssat(&info, &h, &sat, &ssat)) == -1) {
                expn = "Can't read SSAT";
                goto out1;
        }
#ifdef CDF_DEBUG
        cdf_dump_sat("SSAT", &ssat, CDF_SHORT_SEC_SIZE(&h));
#endif

        if ((i = cdf_read_dir(&info, &h, &sat, &dir)) == -1) {
                expn = "Can't read directory";
                goto out2;
        }

        const cdf_directory_t *root_storage;
        if ((i = cdf_read_short_stream(&info, &h, &sat, &dir, &sst,
	    &root_storage)) == -1) {
                expn = "Cannot read short stream";
                goto out3;
        }
#ifdef CDF_DEBUG
        cdf_dump_dir(&info, &h, &sat, &ssat, &sst, &dir);
#endif
#ifdef notdef
	if (root_storage) {
		if (NOTMIME(ms)) {
			char clsbuf[128];
			if (file_printf(ms, "CLSID %s, ",
			    format_clsid(clsbuf, sizeof(clsbuf),
			    root_storage->d_storage_uuid)) == -1)
				return -1;
		}
	}
#endif

        if ((i = cdf_read_summary_info(&info, &h, &sat, &ssat, &sst, &dir,
            &scn)) == -1) {
                if (errno == ESRCH) {
                        corrupt = expn;
                        expn = "No summary info";
                } else {
                        expn = "Cannot read summary info";
                }
                goto out4;
        }
 #ifdef CDF_DEBUG
         cdf_dump_summary_info(&h, &scn);
 #endif
        if ((i = cdf_file_summary_info(ms, &h, &scn, root_storage)) < 0)
            expn = "Can't expand summary_info";
 
 	if (i == 0) {
 		const char *str = NULL;
		cdf_directory_t *d;
		char name[__arraycount(d->d_name)];
		size_t j, k;

		for (j = 0; str == NULL && j < dir.dir_len; j++) {
			d = &dir.dir_tab[j];
			for (k = 0; k < sizeof(name); k++)
				name[k] = (char)cdf_tole2(d->d_name[k]);
			str = cdf_app_to_mime(name,
			    NOTMIME(ms) ? name2desc : name2mime);
		}
		if (NOTMIME(ms)) {
			if (str != NULL) {
				if (file_printf(ms, "%s", str) == -1)
					return -1;
				i = 1;
			}
		} else {
			if (str == NULL)
				str = "vnd.ms-office";
			if (file_printf(ms, "application/%s", str) == -1)
				return -1;
			i = 1;
		}
	}
        free(scn.sst_tab);
out4:
        free(sst.sst_tab);
out3:
        free(dir.dir_tab);
out2:
        free(ssat.sat_tab);
out1:
        free(sat.sat_tab);
out0:
	if (i == -1) {
	    if (NOTMIME(ms)) {
		if (file_printf(ms,
		    "Composite Document File V2 Document") == -1)
		    return -1;
		if (*expn)
		    if (file_printf(ms, ", %s%s", corrupt, expn) == -1)
			return -1;
	    } else {
		if (file_printf(ms, "application/CDFV2-corrupt") == -1)
		    return -1;
	    }
	    i = 1;
	}
        return i;
}
