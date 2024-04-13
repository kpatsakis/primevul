void drm_crtc_init(struct drm_device *dev, struct drm_crtc *crtc,
		   const struct drm_crtc_funcs *funcs)
{
	crtc->dev = dev;
	crtc->funcs = funcs;

	mutex_lock(&dev->mode_config.mutex);
	drm_mode_object_get(dev, &crtc->base, DRM_MODE_OBJECT_CRTC);

	list_add_tail(&crtc->head, &dev->mode_config.crtc_list);
	dev->mode_config.num_crtc++;
	mutex_unlock(&dev->mode_config.mutex);
}
