int drm_mode_create_dvi_i_properties(struct drm_device *dev)
{
	struct drm_property *dvi_i_selector;
	struct drm_property *dvi_i_subconnector;
	int i;

	if (dev->mode_config.dvi_i_select_subconnector_property)
		return 0;

	dvi_i_selector =
		drm_property_create(dev, DRM_MODE_PROP_ENUM,
				    "select subconnector",
				    ARRAY_SIZE(drm_dvi_i_select_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_dvi_i_select_enum_list); i++)
		drm_property_add_enum(dvi_i_selector, i,
				      drm_dvi_i_select_enum_list[i].type,
				      drm_dvi_i_select_enum_list[i].name);
	dev->mode_config.dvi_i_select_subconnector_property = dvi_i_selector;

	dvi_i_subconnector =
		drm_property_create(dev, DRM_MODE_PROP_ENUM |
				    DRM_MODE_PROP_IMMUTABLE,
				    "subconnector",
				    ARRAY_SIZE(drm_dvi_i_subconnector_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_dvi_i_subconnector_enum_list); i++)
		drm_property_add_enum(dvi_i_subconnector, i,
				      drm_dvi_i_subconnector_enum_list[i].type,
				      drm_dvi_i_subconnector_enum_list[i].name);
	dev->mode_config.dvi_i_subconnector_property = dvi_i_subconnector;

	return 0;
}
