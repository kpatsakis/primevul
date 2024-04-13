i915_gem_execbuffer_flush(struct drm_device *dev,
			  uint32_t invalidate_domains,
			  uint32_t flush_domains,
			  uint32_t flush_rings)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	int i, ret;

	if (flush_domains & I915_GEM_DOMAIN_CPU)
		intel_gtt_chipset_flush();

	if (flush_domains & I915_GEM_DOMAIN_GTT)
		wmb();

	if ((flush_domains | invalidate_domains) & I915_GEM_GPU_DOMAINS) {
		for (i = 0; i < I915_NUM_RINGS; i++)
			if (flush_rings & (1 << i)) {
				ret = i915_gem_flush_ring(&dev_priv->ring[i],
							  invalidate_domains,
							  flush_domains);
				if (ret)
					return ret;
			}
	}

	return 0;
}
