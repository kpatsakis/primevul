int drm_mode_cursor_ioctl(struct drm_device *dev,
			void *data, struct drm_file *file_priv)
{
	struct drm_mode_cursor *req = data;
	struct drm_mode_object *obj;
	struct drm_crtc *crtc;
	int ret = 0;

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

	if (!req->flags) {
		DRM_ERROR("no operation set\n");
		return -EINVAL;
	}

	mutex_lock(&dev->mode_config.mutex);
	obj = drm_mode_object_find(dev, req->crtc_id, DRM_MODE_OBJECT_CRTC);
	if (!obj) {
		DRM_DEBUG_KMS("Unknown CRTC ID %d\n", req->crtc_id);
		ret = -EINVAL;
		goto out;
	}
	crtc = obj_to_crtc(obj);

	if (req->flags & DRM_MODE_CURSOR_BO) {
		if (!crtc->funcs->cursor_set) {
			DRM_ERROR("crtc does not support cursor\n");
			ret = -ENXIO;
			goto out;
		}
		/* Turns off the cursor if handle is 0 */
		ret = crtc->funcs->cursor_set(crtc, file_priv, req->handle,
					      req->width, req->height);
	}

	if (req->flags & DRM_MODE_CURSOR_MOVE) {
		if (crtc->funcs->cursor_move) {
			ret = crtc->funcs->cursor_move(crtc, req->x, req->y);
		} else {
			DRM_ERROR("crtc does not support cursor\n");
			ret = -EFAULT;
			goto out;
		}
	}
out:
	mutex_unlock(&dev->mode_config.mutex);
	return ret;
}
