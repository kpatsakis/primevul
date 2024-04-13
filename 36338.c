bool drm_mode_crtc_set_gamma_size(struct drm_crtc *crtc,
				  int gamma_size)
{
	crtc->gamma_size = gamma_size;

	crtc->gamma_store = kzalloc(gamma_size * sizeof(uint16_t) * 3, GFP_KERNEL);
	if (!crtc->gamma_store) {
		crtc->gamma_size = 0;
		return false;
	}

	return true;
}
