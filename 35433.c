static int udf_fill_partdesc_info(struct super_block *sb,
		struct partitionDesc *p, int p_index)
{
	struct udf_part_map *map;
	struct udf_sb_info *sbi = UDF_SB(sb);
	struct partitionHeaderDesc *phd;

	map = &sbi->s_partmaps[p_index];

	map->s_partition_len = le32_to_cpu(p->partitionLength); /* blocks */
	map->s_partition_root = le32_to_cpu(p->partitionStartingLocation);

	if (p->accessType == cpu_to_le32(PD_ACCESS_TYPE_READ_ONLY))
		map->s_partition_flags |= UDF_PART_FLAG_READ_ONLY;
	if (p->accessType == cpu_to_le32(PD_ACCESS_TYPE_WRITE_ONCE))
		map->s_partition_flags |= UDF_PART_FLAG_WRITE_ONCE;
	if (p->accessType == cpu_to_le32(PD_ACCESS_TYPE_REWRITABLE))
		map->s_partition_flags |= UDF_PART_FLAG_REWRITABLE;
	if (p->accessType == cpu_to_le32(PD_ACCESS_TYPE_OVERWRITABLE))
		map->s_partition_flags |= UDF_PART_FLAG_OVERWRITABLE;

	udf_debug("Partition (%d type %x) starts at physical %d, block length %d\n",
		  p_index, map->s_partition_type,
		  map->s_partition_root, map->s_partition_len);

	if (strcmp(p->partitionContents.ident, PD_PARTITION_CONTENTS_NSR02) &&
	    strcmp(p->partitionContents.ident, PD_PARTITION_CONTENTS_NSR03))
		return 0;

	phd = (struct partitionHeaderDesc *)p->partitionContentsUse;
	if (phd->unallocSpaceTable.extLength) {
		struct kernel_lb_addr loc = {
			.logicalBlockNum = le32_to_cpu(
				phd->unallocSpaceTable.extPosition),
			.partitionReferenceNum = p_index,
		};

		map->s_uspace.s_table = udf_iget(sb, &loc);
		if (!map->s_uspace.s_table) {
			udf_debug("cannot load unallocSpaceTable (part %d)\n",
				  p_index);
			return 1;
		}
		map->s_partition_flags |= UDF_PART_FLAG_UNALLOC_TABLE;
		udf_debug("unallocSpaceTable (part %d) @ %ld\n",
			  p_index, map->s_uspace.s_table->i_ino);
	}

	if (phd->unallocSpaceBitmap.extLength) {
		struct udf_bitmap *bitmap = udf_sb_alloc_bitmap(sb, p_index);
		if (!bitmap)
			return 1;
		map->s_uspace.s_bitmap = bitmap;
		bitmap->s_extLength = le32_to_cpu(
				phd->unallocSpaceBitmap.extLength);
		bitmap->s_extPosition = le32_to_cpu(
				phd->unallocSpaceBitmap.extPosition);
		map->s_partition_flags |= UDF_PART_FLAG_UNALLOC_BITMAP;
		udf_debug("unallocSpaceBitmap (part %d) @ %d\n",
			  p_index, bitmap->s_extPosition);
	}

	if (phd->partitionIntegrityTable.extLength)
		udf_debug("partitionIntegrityTable (part %d)\n", p_index);

	if (phd->freedSpaceTable.extLength) {
		struct kernel_lb_addr loc = {
			.logicalBlockNum = le32_to_cpu(
				phd->freedSpaceTable.extPosition),
			.partitionReferenceNum = p_index,
		};

		map->s_fspace.s_table = udf_iget(sb, &loc);
		if (!map->s_fspace.s_table) {
			udf_debug("cannot load freedSpaceTable (part %d)\n",
				  p_index);
			return 1;
		}

		map->s_partition_flags |= UDF_PART_FLAG_FREED_TABLE;
		udf_debug("freedSpaceTable (part %d) @ %ld\n",
			  p_index, map->s_fspace.s_table->i_ino);
	}

	if (phd->freedSpaceBitmap.extLength) {
		struct udf_bitmap *bitmap = udf_sb_alloc_bitmap(sb, p_index);
		if (!bitmap)
			return 1;
		map->s_fspace.s_bitmap = bitmap;
		bitmap->s_extLength = le32_to_cpu(
				phd->freedSpaceBitmap.extLength);
		bitmap->s_extPosition = le32_to_cpu(
				phd->freedSpaceBitmap.extPosition);
		map->s_partition_flags |= UDF_PART_FLAG_FREED_BITMAP;
		udf_debug("freedSpaceBitmap (part %d) @ %d\n",
			  p_index, bitmap->s_extPosition);
	}
	return 0;
}
