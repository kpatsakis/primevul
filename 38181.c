static void map_release(struct kobject *kobj)
{
	struct uio_map *map = to_map(kobj);
	kfree(map);
}
