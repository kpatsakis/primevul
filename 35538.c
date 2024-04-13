i915_gem_execbuffer_reserve(struct intel_ring_buffer *ring,
			    struct drm_file *file,
			    struct list_head *objects)
{
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
	struct drm_i915_gem_object *obj;
	int ret, retry;
	bool has_fenced_gpu_access = INTEL_INFO(ring->dev)->gen < 4;
	struct list_head ordered_objects;

	INIT_LIST_HEAD(&ordered_objects);
	while (!list_empty(objects)) {
		struct drm_i915_gem_exec_object2 *entry;
		bool need_fence, need_mappable;

		obj = list_first_entry(objects,
				       struct drm_i915_gem_object,
				       exec_list);
		entry = obj->exec_entry;

		need_fence =
			has_fenced_gpu_access &&
			entry->flags & EXEC_OBJECT_NEEDS_FENCE &&
			obj->tiling_mode != I915_TILING_NONE;
		need_mappable =
			entry->relocation_count ? true : need_fence;

		if (need_mappable)
			list_move(&obj->exec_list, &ordered_objects);
		else
			list_move_tail(&obj->exec_list, &ordered_objects);

		obj->base.pending_read_domains = 0;
		obj->base.pending_write_domain = 0;
	}
	list_splice(&ordered_objects, objects);

	/* Attempt to pin all of the buffers into the GTT.
	 * This is done in 3 phases:
	 *
	 * 1a. Unbind all objects that do not match the GTT constraints for
	 *     the execbuffer (fenceable, mappable, alignment etc).
	 * 1b. Increment pin count for already bound objects.
	 * 2.  Bind new objects.
	 * 3.  Decrement pin count.
	 *
	 * This avoid unnecessary unbinding of later objects in order to makr
	 * room for the earlier objects *unless* we need to defragment.
	 */
	retry = 0;
	do {
		ret = 0;

		/* Unbind any ill-fitting objects or pin. */
		list_for_each_entry(obj, objects, exec_list) {
			struct drm_i915_gem_exec_object2 *entry = obj->exec_entry;
			bool need_fence, need_mappable;

			if (!obj->gtt_space)
				continue;

			need_fence =
				has_fenced_gpu_access &&
				entry->flags & EXEC_OBJECT_NEEDS_FENCE &&
				obj->tiling_mode != I915_TILING_NONE;
			need_mappable =
				entry->relocation_count ? true : need_fence;

			if ((entry->alignment && obj->gtt_offset & (entry->alignment - 1)) ||
			    (need_mappable && !obj->map_and_fenceable))
				ret = i915_gem_object_unbind(obj);
			else
				ret = pin_and_fence_object(obj, ring);
			if (ret)
				goto err;
		}

		/* Bind fresh objects */
		list_for_each_entry(obj, objects, exec_list) {
			if (obj->gtt_space)
				continue;

			ret = pin_and_fence_object(obj, ring);
			if (ret) {
				int ret_ignore;

				/* This can potentially raise a harmless
				 * -EINVAL if we failed to bind in the above
				 * call. It cannot raise -EINTR since we know
				 * that the bo is freshly bound and so will
				 * not need to be flushed or waited upon.
				 */
				ret_ignore = i915_gem_object_unbind(obj);
				(void)ret_ignore;
				WARN_ON(obj->gtt_space);
				break;
			}
		}

		/* Decrement pin count for bound objects */
		list_for_each_entry(obj, objects, exec_list) {
			struct drm_i915_gem_exec_object2 *entry;

			if (!obj->gtt_space)
				continue;

			entry = obj->exec_entry;
			if (entry->flags & __EXEC_OBJECT_HAS_FENCE) {
				i915_gem_object_unpin_fence(obj);
				entry->flags &= ~__EXEC_OBJECT_HAS_FENCE;
			}

			i915_gem_object_unpin(obj);

			/* ... and ensure ppgtt mapping exist if needed. */
			if (dev_priv->mm.aliasing_ppgtt && !obj->has_aliasing_ppgtt_mapping) {
				i915_ppgtt_bind_object(dev_priv->mm.aliasing_ppgtt,
						       obj, obj->cache_level);

				obj->has_aliasing_ppgtt_mapping = 1;
			}
		}

		if (ret != -ENOSPC || retry > 1)
			return ret;

		/* First attempt, just clear anything that is purgeable.
		 * Second attempt, clear the entire GTT.
		 */
		ret = i915_gem_evict_everything(ring->dev, retry == 0);
		if (ret)
			return ret;

		retry++;
	} while (1);

err:
	list_for_each_entry_continue_reverse(obj, objects, exec_list) {
		struct drm_i915_gem_exec_object2 *entry;

		if (!obj->gtt_space)
			continue;

		entry = obj->exec_entry;
		if (entry->flags & __EXEC_OBJECT_HAS_FENCE) {
			i915_gem_object_unpin_fence(obj);
			entry->flags &= ~__EXEC_OBJECT_HAS_FENCE;
		}

		i915_gem_object_unpin(obj);
	}

	return ret;
}
