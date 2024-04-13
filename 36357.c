int drm_mode_rmfb(struct drm_device *dev,
		   void *data, struct drm_file *file_priv)
{
	struct drm_mode_object *obj;
	struct drm_framebuffer *fb = NULL;
	struct drm_framebuffer *fbl = NULL;
	uint32_t *id = data;
	int ret = 0;
	int found = 0;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

	mutex_lock(&dev->mode_config.mutex);
	obj = drm_mode_object_find(dev, *id, DRM_MODE_OBJECT_FB);
	/* TODO check that we really get a framebuffer back. */
	if (!obj) {
		DRM_ERROR("mode invalid framebuffer id\n");
		ret = -EINVAL;
		goto out;
	}
	fb = obj_to_fb(obj);

	list_for_each_entry(fbl, &file_priv->fbs, filp_head)
		if (fb == fbl)
			found = 1;

	if (!found) {
		DRM_ERROR("tried to remove a fb that we didn't own\n");
		ret = -EINVAL;
		goto out;
	}

	/* TODO release all crtc connected to the framebuffer */
	/* TODO unhock the destructor from the buffer object */

	list_del(&fb->filp_head);
	fb->funcs->destroy(fb);

out:
	mutex_unlock(&dev->mode_config.mutex);
	return ret;
}
