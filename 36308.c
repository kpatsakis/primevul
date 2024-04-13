void drm_connector_init(struct drm_device *dev,
		     struct drm_connector *connector,
		     const struct drm_connector_funcs *funcs,
		     int connector_type)
{
	mutex_lock(&dev->mode_config.mutex);

	connector->dev = dev;
	connector->funcs = funcs;
	drm_mode_object_get(dev, &connector->base, DRM_MODE_OBJECT_CONNECTOR);
	connector->connector_type = connector_type;
	connector->connector_type_id =
		++drm_connector_enum_list[connector_type].count; /* TODO */
	INIT_LIST_HEAD(&connector->user_modes);
	INIT_LIST_HEAD(&connector->probed_modes);
	INIT_LIST_HEAD(&connector->modes);
	connector->edid_blob_ptr = NULL;

	list_add_tail(&connector->head, &dev->mode_config.connector_list);
	dev->mode_config.num_connector++;

	if (connector_type != DRM_MODE_CONNECTOR_VIRTUAL)
		drm_connector_attach_property(connector,
					      dev->mode_config.edid_property,
					      0);

	drm_connector_attach_property(connector,
				      dev->mode_config.dpms_property, 0);

	mutex_unlock(&dev->mode_config.mutex);
}
