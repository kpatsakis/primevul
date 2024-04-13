static void ext4_remove_li_request(struct ext4_li_request *elr)
{
	struct ext4_sb_info *sbi;

	if (!elr)
		return;

	sbi = elr->lr_sbi;

	list_del(&elr->lr_request);
	sbi->s_li_request = NULL;
	kfree(elr);
}
