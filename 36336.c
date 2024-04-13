static int drm_mode_create_standard_connector_properties(struct drm_device *dev)
{
	struct drm_property *edid;
	struct drm_property *dpms;
	int i;

	/*
	 * Standard properties (apply to all connectors)
	 */
	edid = drm_property_create(dev, DRM_MODE_PROP_BLOB |
				   DRM_MODE_PROP_IMMUTABLE,
				   "EDID", 0);
	dev->mode_config.edid_property = edid;

	dpms = drm_property_create(dev, DRM_MODE_PROP_ENUM,
				   "DPMS", ARRAY_SIZE(drm_dpms_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_dpms_enum_list); i++)
		drm_property_add_enum(dpms, i, drm_dpms_enum_list[i].type,
				      drm_dpms_enum_list[i].name);
	dev->mode_config.dpms_property = dpms;

	return 0;
}
