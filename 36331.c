int drm_mode_create_dirty_info_property(struct drm_device *dev)
{
	struct drm_property *dirty_info;
	int i;

	if (dev->mode_config.dirty_info_property)
		return 0;

	dirty_info =
		drm_property_create(dev, DRM_MODE_PROP_ENUM |
				    DRM_MODE_PROP_IMMUTABLE,
				    "dirty",
				    ARRAY_SIZE(drm_dirty_info_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_dirty_info_enum_list); i++)
		drm_property_add_enum(dirty_info, i,
				      drm_dirty_info_enum_list[i].type,
				      drm_dirty_info_enum_list[i].name);
	dev->mode_config.dirty_info_property = dirty_info;

	return 0;
}
