int __drm_set_edid_firmware_path(const char *path)
{
	scnprintf(edid_firmware, sizeof(edid_firmware), "%s", path);

	return 0;
}
