compare_survivor_metric(const void *aa, const void *bb)
{
	const survivor_t *a = aa;
	const survivor_t *b = bb;
	if (a->metric < b->metric) {
		return -1;
	}
	return (a->metric > b->metric);
}
