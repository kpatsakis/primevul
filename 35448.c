static void udf_put_super(struct super_block *sb)
{
	int i;
	struct udf_sb_info *sbi;

	sbi = UDF_SB(sb);

	if (sbi->s_vat_inode)
		iput(sbi->s_vat_inode);
	if (sbi->s_partitions)
		for (i = 0; i < sbi->s_partitions; i++)
			udf_free_partition(&sbi->s_partmaps[i]);
#ifdef CONFIG_UDF_NLS
	if (UDF_QUERY_FLAG(sb, UDF_FLAG_NLS_MAP))
		unload_nls(sbi->s_nls_map);
#endif
	if (!(sb->s_flags & MS_RDONLY))
		udf_close_lvid(sb);
	brelse(sbi->s_lvid_bh);
	kfree(sbi->s_partmaps);
	kfree(sb->s_fs_info);
	sb->s_fs_info = NULL;
}
