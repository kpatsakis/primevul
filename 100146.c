radeon_atombios_encoder_dpms_scratch_regs(struct drm_encoder *encoder, bool on)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_encoder *radeon_encoder = to_radeon_encoder(encoder);
	uint32_t bios_2_scratch;

	if (rdev->family >= CHIP_R600)
		bios_2_scratch = RREG32(R600_BIOS_2_SCRATCH);
	else
		bios_2_scratch = RREG32(RADEON_BIOS_2_SCRATCH);

	if (radeon_encoder->devices & ATOM_DEVICE_TV1_SUPPORT) {
		if (on)
			bios_2_scratch &= ~ATOM_S2_TV1_DPMS_STATE;
		else
			bios_2_scratch |= ATOM_S2_TV1_DPMS_STATE;
	}
	if (radeon_encoder->devices & ATOM_DEVICE_CV_SUPPORT) {
		if (on)
			bios_2_scratch &= ~ATOM_S2_CV_DPMS_STATE;
		else
			bios_2_scratch |= ATOM_S2_CV_DPMS_STATE;
	}
	if (radeon_encoder->devices & ATOM_DEVICE_CRT1_SUPPORT) {
		if (on)
			bios_2_scratch &= ~ATOM_S2_CRT1_DPMS_STATE;
		else
			bios_2_scratch |= ATOM_S2_CRT1_DPMS_STATE;
	}
	if (radeon_encoder->devices & ATOM_DEVICE_CRT2_SUPPORT) {
		if (on)
			bios_2_scratch &= ~ATOM_S2_CRT2_DPMS_STATE;
		else
			bios_2_scratch |= ATOM_S2_CRT2_DPMS_STATE;
	}
	if (radeon_encoder->devices & ATOM_DEVICE_LCD1_SUPPORT) {
		if (on)
			bios_2_scratch &= ~ATOM_S2_LCD1_DPMS_STATE;
		else
			bios_2_scratch |= ATOM_S2_LCD1_DPMS_STATE;
	}
	if (radeon_encoder->devices & ATOM_DEVICE_DFP1_SUPPORT) {
		if (on)
			bios_2_scratch &= ~ATOM_S2_DFP1_DPMS_STATE;
		else
			bios_2_scratch |= ATOM_S2_DFP1_DPMS_STATE;
	}
	if (radeon_encoder->devices & ATOM_DEVICE_DFP2_SUPPORT) {
		if (on)
			bios_2_scratch &= ~ATOM_S2_DFP2_DPMS_STATE;
		else
			bios_2_scratch |= ATOM_S2_DFP2_DPMS_STATE;
	}
	if (radeon_encoder->devices & ATOM_DEVICE_DFP3_SUPPORT) {
		if (on)
			bios_2_scratch &= ~ATOM_S2_DFP3_DPMS_STATE;
		else
			bios_2_scratch |= ATOM_S2_DFP3_DPMS_STATE;
	}
	if (radeon_encoder->devices & ATOM_DEVICE_DFP4_SUPPORT) {
		if (on)
			bios_2_scratch &= ~ATOM_S2_DFP4_DPMS_STATE;
		else
			bios_2_scratch |= ATOM_S2_DFP4_DPMS_STATE;
	}
	if (radeon_encoder->devices & ATOM_DEVICE_DFP5_SUPPORT) {
		if (on)
			bios_2_scratch &= ~ATOM_S2_DFP5_DPMS_STATE;
		else
			bios_2_scratch |= ATOM_S2_DFP5_DPMS_STATE;
	}

	if (rdev->family >= CHIP_R600)
		WREG32(R600_BIOS_2_SCRATCH, bios_2_scratch);
	else
		WREG32(RADEON_BIOS_2_SCRATCH, bios_2_scratch);
}
