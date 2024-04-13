int drm_mode_create_tv_properties(struct drm_device *dev, int num_modes,
				  char *modes[])
{
	struct drm_property *tv_selector;
	struct drm_property *tv_subconnector;
	int i;

	if (dev->mode_config.tv_select_subconnector_property)
		return 0;

	/*
	 * Basic connector properties
	 */
	tv_selector = drm_property_create(dev, DRM_MODE_PROP_ENUM,
					  "select subconnector",
					  ARRAY_SIZE(drm_tv_select_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_tv_select_enum_list); i++)
		drm_property_add_enum(tv_selector, i,
				      drm_tv_select_enum_list[i].type,
				      drm_tv_select_enum_list[i].name);
	dev->mode_config.tv_select_subconnector_property = tv_selector;

	tv_subconnector =
		drm_property_create(dev, DRM_MODE_PROP_ENUM |
				    DRM_MODE_PROP_IMMUTABLE, "subconnector",
				    ARRAY_SIZE(drm_tv_subconnector_enum_list));
	for (i = 0; i < ARRAY_SIZE(drm_tv_subconnector_enum_list); i++)
		drm_property_add_enum(tv_subconnector, i,
				      drm_tv_subconnector_enum_list[i].type,
				      drm_tv_subconnector_enum_list[i].name);
	dev->mode_config.tv_subconnector_property = tv_subconnector;

	/*
	 * Other, TV specific properties: margins & TV modes.
	 */
	dev->mode_config.tv_left_margin_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "left margin", 2);
	dev->mode_config.tv_left_margin_property->values[0] = 0;
	dev->mode_config.tv_left_margin_property->values[1] = 100;

	dev->mode_config.tv_right_margin_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "right margin", 2);
	dev->mode_config.tv_right_margin_property->values[0] = 0;
	dev->mode_config.tv_right_margin_property->values[1] = 100;

	dev->mode_config.tv_top_margin_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "top margin", 2);
	dev->mode_config.tv_top_margin_property->values[0] = 0;
	dev->mode_config.tv_top_margin_property->values[1] = 100;

	dev->mode_config.tv_bottom_margin_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "bottom margin", 2);
	dev->mode_config.tv_bottom_margin_property->values[0] = 0;
	dev->mode_config.tv_bottom_margin_property->values[1] = 100;

	dev->mode_config.tv_mode_property =
		drm_property_create(dev, DRM_MODE_PROP_ENUM,
				    "mode", num_modes);
	for (i = 0; i < num_modes; i++)
		drm_property_add_enum(dev->mode_config.tv_mode_property, i,
				      i, modes[i]);

	dev->mode_config.tv_brightness_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "brightness", 2);
	dev->mode_config.tv_brightness_property->values[0] = 0;
	dev->mode_config.tv_brightness_property->values[1] = 100;

	dev->mode_config.tv_contrast_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "contrast", 2);
	dev->mode_config.tv_contrast_property->values[0] = 0;
	dev->mode_config.tv_contrast_property->values[1] = 100;

	dev->mode_config.tv_flicker_reduction_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "flicker reduction", 2);
	dev->mode_config.tv_flicker_reduction_property->values[0] = 0;
	dev->mode_config.tv_flicker_reduction_property->values[1] = 100;

	dev->mode_config.tv_overscan_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "overscan", 2);
	dev->mode_config.tv_overscan_property->values[0] = 0;
	dev->mode_config.tv_overscan_property->values[1] = 100;

	dev->mode_config.tv_saturation_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "saturation", 2);
	dev->mode_config.tv_saturation_property->values[0] = 0;
	dev->mode_config.tv_saturation_property->values[1] = 100;

	dev->mode_config.tv_hue_property =
		drm_property_create(dev, DRM_MODE_PROP_RANGE,
				    "hue", 2);
	dev->mode_config.tv_hue_property->values[0] = 0;
	dev->mode_config.tv_hue_property->values[1] = 100;

	return 0;
}
