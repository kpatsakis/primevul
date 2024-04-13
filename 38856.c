ip_vs_copy_stats(struct ip_vs_stats_user *dst, struct ip_vs_stats *src)
{
	spin_lock_bh(&src->lock);
	memcpy(dst, &src->ustats, sizeof(*dst));
	spin_unlock_bh(&src->lock);
}
