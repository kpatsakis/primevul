void drm_mode_remove(struct drm_connector *connector,
		     struct drm_display_mode *mode)
{
	list_del(&mode->head);
	kfree(mode);
}
