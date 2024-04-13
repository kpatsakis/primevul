static void ext4_clear_request_list(void)
{
	struct list_head *pos, *n;
	struct ext4_li_request *elr;

	mutex_lock(&ext4_li_info->li_list_mtx);
	list_for_each_safe(pos, n, &ext4_li_info->li_request_list) {
		elr = list_entry(pos, struct ext4_li_request,
				 lr_request);
		ext4_remove_li_request(elr);
	}
	mutex_unlock(&ext4_li_info->li_list_mtx);
}
