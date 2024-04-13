int drm_connector_property_get_value(struct drm_connector *connector,
				  struct drm_property *property, uint64_t *val)
{
	int i;

	for (i = 0; i < DRM_CONNECTOR_MAX_PROPERTY; i++) {
		if (connector->property_ids[i] == property->base.id) {
			*val = connector->property_values[i];
			break;
		}
	}

	if (i == DRM_CONNECTOR_MAX_PROPERTY)
		return -EINVAL;
	return 0;
}
