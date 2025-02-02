static int udf_show_options(struct seq_file *seq, struct dentry *root)
{
	struct super_block *sb = root->d_sb;
	struct udf_sb_info *sbi = UDF_SB(sb);

	if (!UDF_QUERY_FLAG(sb, UDF_FLAG_STRICT))
		seq_puts(seq, ",nostrict");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_BLOCKSIZE_SET))
		seq_printf(seq, ",bs=%lu", sb->s_blocksize);
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UNHIDE))
		seq_puts(seq, ",unhide");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UNDELETE))
		seq_puts(seq, ",undelete");
	if (!UDF_QUERY_FLAG(sb, UDF_FLAG_USE_AD_IN_ICB))
		seq_puts(seq, ",noadinicb");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_USE_SHORT_AD))
		seq_puts(seq, ",shortad");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UID_FORGET))
		seq_puts(seq, ",uid=forget");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UID_IGNORE))
		seq_puts(seq, ",uid=ignore");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_GID_FORGET))
		seq_puts(seq, ",gid=forget");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_GID_IGNORE))
		seq_puts(seq, ",gid=ignore");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UID_SET))
		seq_printf(seq, ",uid=%u", sbi->s_uid);
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_GID_SET))
		seq_printf(seq, ",gid=%u", sbi->s_gid);
	if (sbi->s_umask != 0)
		seq_printf(seq, ",umask=%ho", sbi->s_umask);
	if (sbi->s_fmode != UDF_INVALID_MODE)
		seq_printf(seq, ",mode=%ho", sbi->s_fmode);
	if (sbi->s_dmode != UDF_INVALID_MODE)
		seq_printf(seq, ",dmode=%ho", sbi->s_dmode);
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_SESSION_SET))
		seq_printf(seq, ",session=%u", sbi->s_session);
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_LASTBLOCK_SET))
		seq_printf(seq, ",lastblock=%u", sbi->s_last_block);
	if (sbi->s_anchor != 0)
		seq_printf(seq, ",anchor=%u", sbi->s_anchor);
	/*
	 * volume, partition, fileset and rootdir seem to be ignored
	 * currently
	 */
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_UTF8))
		seq_puts(seq, ",utf8");
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_NLS_MAP) && sbi->s_nls_map)
		seq_printf(seq, ",iocharset=%s", sbi->s_nls_map->charset);

	return 0;
}
