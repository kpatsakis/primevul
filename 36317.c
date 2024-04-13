void drm_fb_release(struct drm_file *priv)
{
	struct drm_device *dev = priv->minor->dev;
	struct drm_framebuffer *fb, *tfb;

	mutex_lock(&dev->mode_config.mutex);
	list_for_each_entry_safe(fb, tfb, &priv->fbs, filp_head) {
		list_del(&fb->filp_head);
		fb->funcs->destroy(fb);
	}
	mutex_unlock(&dev->mode_config.mutex);
}
