void drm_mode_destroy(struct drm_device *dev, struct drm_display_mode *mode)
{
	drm_mode_object_put(dev, &mode->base);

	kfree(mode);
}
