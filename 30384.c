static void *edid_load(struct drm_connector *connector, const char *name,
			const char *connector_name)
{
	const struct firmware *fw = NULL;
	const u8 *fwdata;
	u8 *edid;
	int fwsize, builtin;
	int i, valid_extensions = 0;
	bool print_bad_edid = !connector->bad_edid_counter || (drm_debug & DRM_UT_KMS);

	builtin = match_string(generic_edid_name, GENERIC_EDIDS, name);
	if (builtin >= 0) {
		fwdata = generic_edid[builtin];
		fwsize = sizeof(generic_edid[builtin]);
	} else {
		struct platform_device *pdev;
		int err;

		pdev = platform_device_register_simple(connector_name, -1, NULL, 0);
		if (IS_ERR(pdev)) {
			DRM_ERROR("Failed to register EDID firmware platform device "
				  "for connector \"%s\"\n", connector_name);
			return ERR_CAST(pdev);
		}

		err = request_firmware(&fw, name, &pdev->dev);
		platform_device_unregister(pdev);
		if (err) {
			DRM_ERROR("Requesting EDID firmware \"%s\" failed (err=%d)\n",
				  name, err);
			return ERR_PTR(err);
		}

		fwdata = fw->data;
		fwsize = fw->size;
	}

	if (edid_size(fwdata, fwsize) != fwsize) {
		DRM_ERROR("Size of EDID firmware \"%s\" is invalid "
			  "(expected %d, got %d\n", name,
			  edid_size(fwdata, fwsize), (int)fwsize);
		edid = ERR_PTR(-EINVAL);
		goto out;
	}

	edid = kmemdup(fwdata, fwsize, GFP_KERNEL);
	if (edid == NULL) {
		edid = ERR_PTR(-ENOMEM);
		goto out;
	}

	if (!drm_edid_block_valid(edid, 0, print_bad_edid,
				  &connector->edid_corrupt)) {
		connector->bad_edid_counter++;
		DRM_ERROR("Base block of EDID firmware \"%s\" is invalid ",
		    name);
		kfree(edid);
		edid = ERR_PTR(-EINVAL);
		goto out;
	}

	for (i = 1; i <= edid[0x7e]; i++) {
		if (i != valid_extensions + 1)
			memcpy(edid + (valid_extensions + 1) * EDID_LENGTH,
			    edid + i * EDID_LENGTH, EDID_LENGTH);
		if (drm_edid_block_valid(edid + i * EDID_LENGTH, i,
					 print_bad_edid,
					 NULL))
			valid_extensions++;
	}

	if (valid_extensions != edid[0x7e]) {
		u8 *new_edid;

		edid[EDID_LENGTH-1] += edid[0x7e] - valid_extensions;
		DRM_INFO("Found %d valid extensions instead of %d in EDID data "
		    "\"%s\" for connector \"%s\"\n", valid_extensions,
		    edid[0x7e], name, connector_name);
		edid[0x7e] = valid_extensions;

		new_edid = krealloc(edid, (valid_extensions + 1) * EDID_LENGTH,
				    GFP_KERNEL);
		if (new_edid)
			edid = new_edid;
	}

	DRM_INFO("Got %s EDID base block and %d extension%s from "
	    "\"%s\" for connector \"%s\"\n", (builtin >= 0) ? "built-in" :
	    "external", valid_extensions, valid_extensions == 1 ? "" : "s",
	    name, connector_name);

out:
	release_firmware(fw);
	return edid;
}
