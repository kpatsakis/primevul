static void udf_load_logicalvolint(struct super_block *sb, struct kernel_extent_ad loc)
{
	struct buffer_head *bh = NULL;
	uint16_t ident;
	struct udf_sb_info *sbi = UDF_SB(sb);
	struct logicalVolIntegrityDesc *lvid;

	while (loc.extLength > 0 &&
	       (bh = udf_read_tagged(sb, loc.extLocation,
				     loc.extLocation, &ident)) &&
	       ident == TAG_IDENT_LVID) {
		sbi->s_lvid_bh = bh;
		lvid = (struct logicalVolIntegrityDesc *)bh->b_data;

		if (lvid->nextIntegrityExt.extLength)
			udf_load_logicalvolint(sb,
				leea_to_cpu(lvid->nextIntegrityExt));

		if (sbi->s_lvid_bh != bh)
			brelse(bh);
		loc.extLength -= sb->s_blocksize;
		loc.extLocation++;
	}
	if (sbi->s_lvid_bh != bh)
		brelse(bh);
}
