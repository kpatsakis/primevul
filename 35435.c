struct inode *udf_find_metadata_inode_efe(struct super_block *sb,
					u32 meta_file_loc, u32 partition_num)
{
	struct kernel_lb_addr addr;
	struct inode *metadata_fe;

	addr.logicalBlockNum = meta_file_loc;
	addr.partitionReferenceNum = partition_num;

	metadata_fe = udf_iget(sb, &addr);

	if (metadata_fe == NULL)
		udf_warn(sb, "metadata inode efe not found\n");
	else if (UDF_I(metadata_fe)->i_alloc_type != ICBTAG_FLAG_AD_SHORT) {
		udf_warn(sb, "metadata inode efe does not have short allocation descriptors!\n");
		iput(metadata_fe);
		metadata_fe = NULL;
	}

	return metadata_fe;
}
