static int drm_mode_attachmode(struct drm_device *dev,
			       struct drm_connector *connector,
			       struct drm_display_mode *mode)
{
	int ret = 0;

	list_add_tail(&mode->head, &connector->user_modes);
	return ret;
}
