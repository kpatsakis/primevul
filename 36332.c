int drm_mode_create_dithering_property(struct drm_device *dev)
{
	struct drm_property *dithering_mode;
	int i;

	if (dev->mode_config.dithering_mode_property)
		return 0;

	dithering_mode =
		drm_property_create(dev, DRM_MODE_PROP_ENUM, "dithering",
				    ARRAY_SIZE(drm_dithering_mode_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_dithering_mode_enum_list); i++)
		drm_property_add_enum(dithering_mode, i,
				      drm_dithering_mode_enum_list[i].type,
				      drm_dithering_mode_enum_list[i].name);
	dev->mode_config.dithering_mode_property = dithering_mode;

	return 0;
}
