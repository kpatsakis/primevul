eb_create(int size)
{
	struct eb_objects *eb;
	int count = PAGE_SIZE / sizeof(struct hlist_head) / 2;
	while (count > size)
		count >>= 1;
	eb = kzalloc(count*sizeof(struct hlist_head) +
		     sizeof(struct eb_objects),
		     GFP_KERNEL);
	if (eb == NULL)
		return eb;

	eb->and = count - 1;
	return eb;
}
