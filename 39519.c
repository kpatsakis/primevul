ftrace_allocate_pages(unsigned long num_to_init)
{
	struct ftrace_page *start_pg;
	struct ftrace_page *pg;
	int order;
	int cnt;

	if (!num_to_init)
		return 0;

	start_pg = pg = kzalloc(sizeof(*pg), GFP_KERNEL);
	if (!pg)
		return NULL;

	/*
	 * Try to allocate as much as possible in one continues
	 * location that fills in all of the space. We want to
	 * waste as little space as possible.
	 */
	for (;;) {
		cnt = ftrace_allocate_records(pg, num_to_init);
		if (cnt < 0)
			goto free_pages;

		num_to_init -= cnt;
		if (!num_to_init)
			break;

		pg->next = kzalloc(sizeof(*pg), GFP_KERNEL);
		if (!pg->next)
			goto free_pages;

		pg = pg->next;
	}

	return start_pg;

 free_pages:
	while (start_pg) {
		order = get_count_order(pg->size / ENTRIES_PER_PAGE);
		free_pages((unsigned long)pg->records, order);
		start_pg = pg->next;
		kfree(pg);
		pg = start_pg;
	}
	pr_info("ftrace: FAILED to allocate memory for functions\n");
	return NULL;
}
