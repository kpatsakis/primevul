i915_gem_check_execbuffer(struct drm_i915_gem_execbuffer2 *exec)
{
	return ((exec->batch_start_offset | exec->batch_len) & 0x7) == 0;
}
