char *drm_get_encoder_name(struct drm_encoder *encoder)
{
	static char buf[32];

	snprintf(buf, 32, "%s-%d",
		 drm_encoder_enum_list[encoder->encoder_type].name,
		 encoder->base.id);
	return buf;
}
