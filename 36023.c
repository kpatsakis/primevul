static void ext4_unregister_li_request(struct super_block *sb)
{
	mutex_lock(&ext4_li_mtx);
	if (!ext4_li_info) {
		mutex_unlock(&ext4_li_mtx);
		return;
	}

	mutex_lock(&ext4_li_info->li_list_mtx);
	ext4_remove_li_request(EXT4_SB(sb)->s_li_request);
	mutex_unlock(&ext4_li_info->li_list_mtx);
	mutex_unlock(&ext4_li_mtx);
}
