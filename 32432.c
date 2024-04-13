compare_point_edge(const void *aa, const void *bb)
{
	const point_t *a = aa;
	const point_t *b = bb;
	if (a->edge < b->edge) {
		return -1;
	}
	return (a->edge > b->edge);
}
