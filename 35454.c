static sector_t udf_scan_anchors(struct super_block *sb, sector_t lastblock,
				 struct kernel_lb_addr *fileset)
{
	sector_t last[6];
	int i;
	struct udf_sb_info *sbi = UDF_SB(sb);
	int last_count = 0;

	/* First try user provided anchor */
	if (sbi->s_anchor) {
		if (udf_check_anchor_block(sb, sbi->s_anchor, fileset))
			return lastblock;
	}
	/*
	 * according to spec, anchor is in either:
	 *     block 256
	 *     lastblock-256
	 *     lastblock
	 *  however, if the disc isn't closed, it could be 512.
	 */
	if (udf_check_anchor_block(sb, sbi->s_session + 256, fileset))
		return lastblock;
	/*
	 * The trouble is which block is the last one. Drives often misreport
	 * this so we try various possibilities.
	 */
	last[last_count++] = lastblock;
	if (lastblock >= 1)
		last[last_count++] = lastblock - 1;
	last[last_count++] = lastblock + 1;
	if (lastblock >= 2)
		last[last_count++] = lastblock - 2;
	if (lastblock >= 150)
		last[last_count++] = lastblock - 150;
	if (lastblock >= 152)
		last[last_count++] = lastblock - 152;

	for (i = 0; i < last_count; i++) {
		if (last[i] >= sb->s_bdev->bd_inode->i_size >>
				sb->s_blocksize_bits)
			continue;
		if (udf_check_anchor_block(sb, last[i], fileset))
			return last[i];
		if (last[i] < 256)
			continue;
		if (udf_check_anchor_block(sb, last[i] - 256, fileset))
			return last[i];
	}

	/* Finally try block 512 in case media is open */
	if (udf_check_anchor_block(sb, sbi->s_session + 512, fileset))
		return last[0];
	return 0;
}
