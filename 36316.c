void drm_encoder_init(struct drm_device *dev,
		      struct drm_encoder *encoder,
		      const struct drm_encoder_funcs *funcs,
		      int encoder_type)
{
	mutex_lock(&dev->mode_config.mutex);

	encoder->dev = dev;

	drm_mode_object_get(dev, &encoder->base, DRM_MODE_OBJECT_ENCODER);
	encoder->encoder_type = encoder_type;
	encoder->funcs = funcs;

	list_add_tail(&encoder->head, &dev->mode_config.encoder_list);
	dev->mode_config.num_encoder++;

	mutex_unlock(&dev->mode_config.mutex);
}
