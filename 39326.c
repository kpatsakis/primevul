static u32 map_id_range_down(struct uid_gid_map *map, u32 id, u32 count)
{
	unsigned idx, extents;
	u32 first, last, id2;

	id2 = id + count - 1;

	/* Find the matching extent */
	extents = map->nr_extents;
	smp_read_barrier_depends();
	for (idx = 0; idx < extents; idx++) {
		first = map->extent[idx].first;
		last = first + map->extent[idx].count - 1;
		if (id >= first && id <= last &&
		    (id2 >= first && id2 <= last))
			break;
	}
	/* Map the id or note failure */
	if (idx < extents)
		id = (id - first) + map->extent[idx].lower_first;
	else
		id = (u32) -1;

	return id;
}
