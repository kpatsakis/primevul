static bool ldm_parse_tocblock (const u8 *data, struct tocblock *toc)
{
	BUG_ON (!data || !toc);

	if (MAGIC_TOCBLOCK != get_unaligned_be64(data)) {
		ldm_crit ("Cannot find TOCBLOCK, database may be corrupt.");
		return false;
	}
	strncpy (toc->bitmap1_name, data + 0x24, sizeof (toc->bitmap1_name));
	toc->bitmap1_name[sizeof (toc->bitmap1_name) - 1] = 0;
	toc->bitmap1_start = get_unaligned_be64(data + 0x2E);
	toc->bitmap1_size  = get_unaligned_be64(data + 0x36);

	if (strncmp (toc->bitmap1_name, TOC_BITMAP1,
			sizeof (toc->bitmap1_name)) != 0) {
		ldm_crit ("TOCBLOCK's first bitmap is '%s', should be '%s'.",
				TOC_BITMAP1, toc->bitmap1_name);
		return false;
	}
	strncpy (toc->bitmap2_name, data + 0x46, sizeof (toc->bitmap2_name));
	toc->bitmap2_name[sizeof (toc->bitmap2_name) - 1] = 0;
	toc->bitmap2_start = get_unaligned_be64(data + 0x50);
	toc->bitmap2_size  = get_unaligned_be64(data + 0x58);
	if (strncmp (toc->bitmap2_name, TOC_BITMAP2,
			sizeof (toc->bitmap2_name)) != 0) {
		ldm_crit ("TOCBLOCK's second bitmap is '%s', should be '%s'.",
				TOC_BITMAP2, toc->bitmap2_name);
		return false;
	}
	ldm_debug ("Parsed TOCBLOCK successfully.");
	return true;
}
