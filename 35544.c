intel_enable_semaphores(struct drm_device *dev)
{
	if (INTEL_INFO(dev)->gen < 6)
		return 0;

	if (i915_semaphores >= 0)
		return i915_semaphores;

	/* Disable semaphores on SNB */
	if (INTEL_INFO(dev)->gen == 6)
		return 0;

	return 1;
}
