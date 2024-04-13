eb_reset(struct eb_objects *eb)
{
	memset(eb->buckets, 0, (eb->and+1)*sizeof(struct hlist_head));
}
