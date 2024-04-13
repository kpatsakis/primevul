void drm_connector_cleanup(struct drm_connector *connector)
{
	struct drm_device *dev = connector->dev;
	struct drm_display_mode *mode, *t;

	list_for_each_entry_safe(mode, t, &connector->probed_modes, head)
		drm_mode_remove(connector, mode);

	list_for_each_entry_safe(mode, t, &connector->modes, head)
		drm_mode_remove(connector, mode);

	list_for_each_entry_safe(mode, t, &connector->user_modes, head)
		drm_mode_remove(connector, mode);

	mutex_lock(&dev->mode_config.mutex);
	drm_mode_object_put(dev, &connector->base);
	list_del(&connector->head);
	dev->mode_config.num_connector--;
	mutex_unlock(&dev->mode_config.mutex);
}
