i915_gem_do_execbuffer(struct drm_device *dev, void *data,
		       struct drm_file *file,
		       struct drm_i915_gem_execbuffer2 *args,
		       struct drm_i915_gem_exec_object2 *exec)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct list_head objects;
	struct eb_objects *eb;
	struct drm_i915_gem_object *batch_obj;
	struct drm_clip_rect *cliprects = NULL;
	struct intel_ring_buffer *ring;
	u32 exec_start, exec_len;
	u32 seqno;
	u32 mask;
	int ret, mode, i;

	if (!i915_gem_check_execbuffer(args)) {
		DRM_DEBUG("execbuf with invalid offset/length\n");
		return -EINVAL;
	}

	ret = validate_exec_list(exec, args->buffer_count);
	if (ret)
		return ret;

	switch (args->flags & I915_EXEC_RING_MASK) {
	case I915_EXEC_DEFAULT:
	case I915_EXEC_RENDER:
		ring = &dev_priv->ring[RCS];
		break;
	case I915_EXEC_BSD:
		if (!HAS_BSD(dev)) {
			DRM_DEBUG("execbuf with invalid ring (BSD)\n");
			return -EINVAL;
		}
		ring = &dev_priv->ring[VCS];
		break;
	case I915_EXEC_BLT:
		if (!HAS_BLT(dev)) {
			DRM_DEBUG("execbuf with invalid ring (BLT)\n");
			return -EINVAL;
		}
		ring = &dev_priv->ring[BCS];
		break;
	default:
		DRM_DEBUG("execbuf with unknown ring: %d\n",
			  (int)(args->flags & I915_EXEC_RING_MASK));
		return -EINVAL;
	}

	mode = args->flags & I915_EXEC_CONSTANTS_MASK;
	mask = I915_EXEC_CONSTANTS_MASK;
	switch (mode) {
	case I915_EXEC_CONSTANTS_REL_GENERAL:
	case I915_EXEC_CONSTANTS_ABSOLUTE:
	case I915_EXEC_CONSTANTS_REL_SURFACE:
		if (ring == &dev_priv->ring[RCS] &&
		    mode != dev_priv->relative_constants_mode) {
			if (INTEL_INFO(dev)->gen < 4)
				return -EINVAL;

			if (INTEL_INFO(dev)->gen > 5 &&
			    mode == I915_EXEC_CONSTANTS_REL_SURFACE)
				return -EINVAL;

			/* The HW changed the meaning on this bit on gen6 */
			if (INTEL_INFO(dev)->gen >= 6)
				mask &= ~I915_EXEC_CONSTANTS_REL_SURFACE;
		}
		break;
	default:
		DRM_DEBUG("execbuf with unknown constants: %d\n", mode);
		return -EINVAL;
	}

	if (args->buffer_count < 1) {
		DRM_DEBUG("execbuf with %d buffers\n", args->buffer_count);
		return -EINVAL;
	}

	if (args->num_cliprects != 0) {
		if (ring != &dev_priv->ring[RCS]) {
			DRM_DEBUG("clip rectangles are only valid with the render ring\n");
 			return -EINVAL;
 		}
 
		if (args->num_cliprects > UINT_MAX / sizeof(*cliprects)) {
			DRM_DEBUG("execbuf with %u cliprects\n",
				  args->num_cliprects);
			return -EINVAL;
		}
 		cliprects = kmalloc(args->num_cliprects * sizeof(*cliprects),
 				    GFP_KERNEL);
 		if (cliprects == NULL) {
			ret = -ENOMEM;
			goto pre_mutex_err;
		}

		if (copy_from_user(cliprects,
				     (struct drm_clip_rect __user *)(uintptr_t)
				     args->cliprects_ptr,
				     sizeof(*cliprects)*args->num_cliprects)) {
			ret = -EFAULT;
			goto pre_mutex_err;
		}
	}

	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		goto pre_mutex_err;

	if (dev_priv->mm.suspended) {
		mutex_unlock(&dev->struct_mutex);
		ret = -EBUSY;
		goto pre_mutex_err;
	}

	eb = eb_create(args->buffer_count);
	if (eb == NULL) {
		mutex_unlock(&dev->struct_mutex);
		ret = -ENOMEM;
		goto pre_mutex_err;
	}

	/* Look up object handles */
	INIT_LIST_HEAD(&objects);
	for (i = 0; i < args->buffer_count; i++) {
		struct drm_i915_gem_object *obj;

		obj = to_intel_bo(drm_gem_object_lookup(dev, file,
							exec[i].handle));
		if (&obj->base == NULL) {
			DRM_DEBUG("Invalid object handle %d at index %d\n",
				   exec[i].handle, i);
			/* prevent error path from reading uninitialized data */
			ret = -ENOENT;
			goto err;
		}

		if (!list_empty(&obj->exec_list)) {
			DRM_DEBUG("Object %p [handle %d, index %d] appears more than once in object list\n",
				   obj, exec[i].handle, i);
			ret = -EINVAL;
			goto err;
		}

		list_add_tail(&obj->exec_list, &objects);
		obj->exec_handle = exec[i].handle;
		obj->exec_entry = &exec[i];
		eb_add_object(eb, obj);
	}

	/* take note of the batch buffer before we might reorder the lists */
	batch_obj = list_entry(objects.prev,
			       struct drm_i915_gem_object,
			       exec_list);

	/* Move the objects en-masse into the GTT, evicting if necessary. */
	ret = i915_gem_execbuffer_reserve(ring, file, &objects);
	if (ret)
		goto err;

	/* The objects are in their final locations, apply the relocations. */
	ret = i915_gem_execbuffer_relocate(dev, eb, &objects);
	if (ret) {
		if (ret == -EFAULT) {
			ret = i915_gem_execbuffer_relocate_slow(dev, file, ring,
								&objects, eb,
								exec,
								args->buffer_count);
			BUG_ON(!mutex_is_locked(&dev->struct_mutex));
		}
		if (ret)
			goto err;
	}

	/* Set the pending read domains for the batch buffer to COMMAND */
	if (batch_obj->base.pending_write_domain) {
		DRM_DEBUG("Attempting to use self-modifying batch buffer\n");
		ret = -EINVAL;
		goto err;
	}
	batch_obj->base.pending_read_domains |= I915_GEM_DOMAIN_COMMAND;

	ret = i915_gem_execbuffer_move_to_gpu(ring, &objects);
	if (ret)
		goto err;

	seqno = i915_gem_next_request_seqno(ring);
	for (i = 0; i < ARRAY_SIZE(ring->sync_seqno); i++) {
		if (seqno < ring->sync_seqno[i]) {
			/* The GPU can not handle its semaphore value wrapping,
			 * so every billion or so execbuffers, we need to stall
			 * the GPU in order to reset the counters.
			 */
			ret = i915_gpu_idle(dev, true);
			if (ret)
				goto err;

			BUG_ON(ring->sync_seqno[i]);
		}
	}

	if (ring == &dev_priv->ring[RCS] &&
	    mode != dev_priv->relative_constants_mode) {
		ret = intel_ring_begin(ring, 4);
		if (ret)
				goto err;

		intel_ring_emit(ring, MI_NOOP);
		intel_ring_emit(ring, MI_LOAD_REGISTER_IMM(1));
		intel_ring_emit(ring, INSTPM);
		intel_ring_emit(ring, mask << 16 | mode);
		intel_ring_advance(ring);

		dev_priv->relative_constants_mode = mode;
	}

	if (args->flags & I915_EXEC_GEN7_SOL_RESET) {
		ret = i915_reset_gen7_sol_offsets(dev, ring);
		if (ret)
			goto err;
	}

	trace_i915_gem_ring_dispatch(ring, seqno);

	exec_start = batch_obj->gtt_offset + args->batch_start_offset;
	exec_len = args->batch_len;
	if (cliprects) {
		for (i = 0; i < args->num_cliprects; i++) {
			ret = i915_emit_box(dev, &cliprects[i],
					    args->DR1, args->DR4);
			if (ret)
				goto err;

			ret = ring->dispatch_execbuffer(ring,
							exec_start, exec_len);
			if (ret)
				goto err;
		}
	} else {
		ret = ring->dispatch_execbuffer(ring, exec_start, exec_len);
		if (ret)
			goto err;
	}

	i915_gem_execbuffer_move_to_active(&objects, ring, seqno);
	i915_gem_execbuffer_retire_commands(dev, file, ring);

err:
	eb_destroy(eb);
	while (!list_empty(&objects)) {
		struct drm_i915_gem_object *obj;

		obj = list_first_entry(&objects,
				       struct drm_i915_gem_object,
				       exec_list);
		list_del_init(&obj->exec_list);
		drm_gem_object_unreference(&obj->base);
	}

	mutex_unlock(&dev->struct_mutex);

pre_mutex_err:
	kfree(cliprects);
	return ret;
}
