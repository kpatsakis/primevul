static int udf_load_metadata_files(struct super_block *sb, int partition)
{
	struct udf_sb_info *sbi = UDF_SB(sb);
	struct udf_part_map *map;
	struct udf_meta_data *mdata;
	struct kernel_lb_addr addr;

	map = &sbi->s_partmaps[partition];
	mdata = &map->s_type_specific.s_metadata;

	/* metadata address */
	udf_debug("Metadata file location: block = %d part = %d\n",
		  mdata->s_meta_file_loc, map->s_partition_num);

	mdata->s_metadata_fe = udf_find_metadata_inode_efe(sb,
		mdata->s_meta_file_loc, map->s_partition_num);

	if (mdata->s_metadata_fe == NULL) {
		/* mirror file entry */
		udf_debug("Mirror metadata file location: block = %d part = %d\n",
			  mdata->s_mirror_file_loc, map->s_partition_num);

		mdata->s_mirror_fe = udf_find_metadata_inode_efe(sb,
			mdata->s_mirror_file_loc, map->s_partition_num);

		if (mdata->s_mirror_fe == NULL) {
			udf_err(sb, "Both metadata and mirror metadata inode efe can not found\n");
			goto error_exit;
		}
	}

	/*
	 * bitmap file entry
	 * Note:
	 * Load only if bitmap file location differs from 0xFFFFFFFF (DCN-5102)
	*/
	if (mdata->s_bitmap_file_loc != 0xFFFFFFFF) {
		addr.logicalBlockNum = mdata->s_bitmap_file_loc;
		addr.partitionReferenceNum = map->s_partition_num;

		udf_debug("Bitmap file location: block = %d part = %d\n",
			  addr.logicalBlockNum, addr.partitionReferenceNum);

		mdata->s_bitmap_fe = udf_iget(sb, &addr);

		if (mdata->s_bitmap_fe == NULL) {
			if (sb->s_flags & MS_RDONLY)
				udf_warn(sb, "bitmap inode efe not found but it's ok since the disc is mounted read-only\n");
			else {
				udf_err(sb, "bitmap inode efe not found and attempted read-write mount\n");
				goto error_exit;
			}
		}
	}

	udf_debug("udf_load_metadata_files Ok\n");

	return 0;

error_exit:
	return 1;
}
