int __drm_get_edid_firmware_path(char *buf, size_t bufsize)
{
	return scnprintf(buf, bufsize, "%s", edid_firmware);
}
