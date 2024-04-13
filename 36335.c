int drm_mode_create_scaling_mode_property(struct drm_device *dev)
{
	struct drm_property *scaling_mode;
	int i;

	if (dev->mode_config.scaling_mode_property)
		return 0;

	scaling_mode =
		drm_property_create(dev, DRM_MODE_PROP_ENUM, "scaling mode",
				    ARRAY_SIZE(drm_scaling_mode_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_scaling_mode_enum_list); i++)
		drm_property_add_enum(scaling_mode, i,
				      drm_scaling_mode_enum_list[i].type,
				      drm_scaling_mode_enum_list[i].name);

	dev->mode_config.scaling_mode_property = scaling_mode;

	return 0;
}
