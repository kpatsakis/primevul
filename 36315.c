void drm_encoder_cleanup(struct drm_encoder *encoder)
{
	struct drm_device *dev = encoder->dev;
	mutex_lock(&dev->mode_config.mutex);
	drm_mode_object_put(dev, &encoder->base);
	list_del(&encoder->head);
	dev->mode_config.num_encoder--;
	mutex_unlock(&dev->mode_config.mutex);
}
