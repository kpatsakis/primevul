eb_add_object(struct eb_objects *eb, struct drm_i915_gem_object *obj)
{
	hlist_add_head(&obj->exec_node,
		       &eb->buckets[obj->exec_handle & eb->and]);
}
