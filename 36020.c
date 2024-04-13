static int ext4_statfs(struct dentry *dentry, struct kstatfs *buf)
{
	struct super_block *sb = dentry->d_sb;
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_super_block *es = sbi->s_es;
	struct ext4_group_desc *gdp;
	u64 fsid;
	s64 bfree;

	if (test_opt(sb, MINIX_DF)) {
		sbi->s_overhead_last = 0;
	} else if (es->s_overhead_clusters) {
		sbi->s_overhead_last = le32_to_cpu(es->s_overhead_clusters);
	} else if (sbi->s_blocks_last != ext4_blocks_count(es)) {
		ext4_group_t i, ngroups = ext4_get_groups_count(sb);
		ext4_fsblk_t overhead = 0;

		/*
		 * Compute the overhead (FS structures).  This is constant
		 * for a given filesystem unless the number of block groups
		 * changes so we cache the previous value until it does.
		 */

		/*
		 * All of the blocks before first_data_block are
		 * overhead
		 */
		overhead = EXT4_B2C(sbi, le32_to_cpu(es->s_first_data_block));

		/*
		 * Add the overhead found in each block group
		 */
		for (i = 0; i < ngroups; i++) {
			gdp = ext4_get_group_desc(sb, i, NULL);
			overhead += ext4_num_overhead_clusters(sb, i, gdp);
			cond_resched();
		}
		sbi->s_overhead_last = overhead;
		smp_wmb();
		sbi->s_blocks_last = ext4_blocks_count(es);
	}

	buf->f_type = EXT4_SUPER_MAGIC;
	buf->f_bsize = sb->s_blocksize;
	buf->f_blocks = (ext4_blocks_count(es) -
			 EXT4_C2B(sbi, sbi->s_overhead_last));
	bfree = percpu_counter_sum_positive(&sbi->s_freeclusters_counter) -
		percpu_counter_sum_positive(&sbi->s_dirtyclusters_counter);
	/* prevent underflow in case that few free space is available */
	buf->f_bfree = EXT4_C2B(sbi, max_t(s64, bfree, 0));
	buf->f_bavail = buf->f_bfree - ext4_r_blocks_count(es);
	if (buf->f_bfree < ext4_r_blocks_count(es))
		buf->f_bavail = 0;
	buf->f_files = le32_to_cpu(es->s_inodes_count);
	buf->f_ffree = percpu_counter_sum_positive(&sbi->s_freeinodes_counter);
	buf->f_namelen = EXT4_NAME_LEN;
	fsid = le64_to_cpup((void *)es->s_uuid) ^
	       le64_to_cpup((void *)es->s_uuid + sizeof(u64));
	buf->f_fsid.val[0] = fsid & 0xFFFFFFFFUL;
	buf->f_fsid.val[1] = (fsid >> 32) & 0xFFFFFFFFUL;

	return 0;
}
