void drm_crtc_cleanup(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;

	if (crtc->gamma_store) {
		kfree(crtc->gamma_store);
		crtc->gamma_store = NULL;
	}

	drm_mode_object_put(dev, &crtc->base);
	list_del(&crtc->head);
	dev->mode_config.num_crtc--;
}
