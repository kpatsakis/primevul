i915_gem_execbuffer_sync_rings(struct drm_i915_gem_object *obj,
			       struct intel_ring_buffer *to)
{
	struct intel_ring_buffer *from = obj->ring;
	u32 seqno;
	int ret, idx;

	if (from == NULL || to == from)
		return 0;

	/* XXX gpu semaphores are implicated in various hard hangs on SNB */
	if (!intel_enable_semaphores(obj->base.dev))
		return i915_gem_object_wait_rendering(obj);

	idx = intel_ring_sync_index(from, to);

	seqno = obj->last_rendering_seqno;
	if (seqno <= from->sync_seqno[idx])
		return 0;

	if (seqno == from->outstanding_lazy_request) {
		struct drm_i915_gem_request *request;

		request = kzalloc(sizeof(*request), GFP_KERNEL);
		if (request == NULL)
			return -ENOMEM;

		ret = i915_add_request(from, NULL, request);
		if (ret) {
			kfree(request);
			return ret;
		}

		seqno = request->seqno;
	}

	from->sync_seqno[idx] = seqno;

	return to->sync_to(to, from, seqno - 1);
}
