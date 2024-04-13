static struct drm_property_blob *drm_property_create_blob(struct drm_device *dev, int length,
							  void *data)
{
	struct drm_property_blob *blob;

	if (!length || !data)
		return NULL;

	blob = kzalloc(sizeof(struct drm_property_blob)+length, GFP_KERNEL);
	if (!blob)
		return NULL;

	blob->data = (void *)((char *)blob + sizeof(struct drm_property_blob));
	blob->length = length;

	memcpy(blob->data, data, length);

	drm_mode_object_get(dev, &blob->base, DRM_MODE_OBJECT_BLOB);

	list_add_tail(&blob->head, &dev->mode_config.property_blob_list);
	return blob;
}
