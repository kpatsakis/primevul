void drm_framebuffer_cleanup(struct drm_framebuffer *fb)
{
	struct drm_device *dev = fb->dev;
	struct drm_crtc *crtc;
	struct drm_mode_set set;
	int ret;

	/* remove from any CRTC */
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		if (crtc->fb == fb) {
			/* should turn off the crtc */
			memset(&set, 0, sizeof(struct drm_mode_set));
			set.crtc = crtc;
			set.fb = NULL;
			ret = crtc->funcs->set_config(&set);
			if (ret)
				DRM_ERROR("failed to reset crtc %p when fb was deleted\n", crtc);
		}
	}

	drm_mode_object_put(dev, &fb->base);
	list_del(&fb->head);
	dev->mode_config.num_fb--;
}
