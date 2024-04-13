int sja1105_static_config_upload(struct sja1105_private *priv)
{
	unsigned long port_bitmap = GENMASK_ULL(SJA1105_NUM_PORTS - 1, 0);
	struct sja1105_static_config *config = &priv->static_config;
	const struct sja1105_regs *regs = priv->info->regs;
	struct device *dev = &priv->spidev->dev;
	struct sja1105_status status;
	int rc, retries = RETRIES;
	u8 *config_buf;
	int buf_len;

	buf_len = sja1105_static_config_get_length(config);
	config_buf = kcalloc(buf_len, sizeof(char), GFP_KERNEL);
	if (!config_buf)
		return -ENOMEM;

	rc = static_config_buf_prepare_for_upload(priv, config_buf, buf_len);
	if (rc < 0) {
		dev_err(dev, "Invalid config, cannot upload\n");
		return -EINVAL;
	}
	/* Prevent PHY jabbering during switch reset by inhibiting
	 * Tx on all ports and waiting for current packet to drain.
	 * Otherwise, the PHY will see an unterminated Ethernet packet.
	 */
	rc = sja1105_inhibit_tx(priv, port_bitmap, true);
	if (rc < 0) {
		dev_err(dev, "Failed to inhibit Tx on ports\n");
		return -ENXIO;
	}
	/* Wait for an eventual egress packet to finish transmission
	 * (reach IFG). It is guaranteed that a second one will not
	 * follow, and that switch cold reset is thus safe
	 */
	usleep_range(500, 1000);
	do {
		/* Put the SJA1105 in programming mode */
		rc = sja1105_cold_reset(priv);
		if (rc < 0) {
			dev_err(dev, "Failed to reset switch, retrying...\n");
			continue;
		}
		/* Wait for the switch to come out of reset */
		usleep_range(1000, 5000);
		/* Upload the static config to the device */
		rc = sja1105_spi_send_long_packed_buf(priv, SPI_WRITE,
						      regs->config,
						      config_buf, buf_len);
		if (rc < 0) {
			dev_err(dev, "Failed to upload config, retrying...\n");
			continue;
		}
		/* Check that SJA1105 responded well to the config upload */
		rc = sja1105_status_get(priv, &status);
		if (rc < 0)
			continue;

		if (status.ids == 1) {
			dev_err(dev, "Mismatch between hardware and static config "
				"device id. Wrote 0x%llx, wants 0x%llx\n",
				config->device_id, priv->info->device_id);
			continue;
		}
		if (status.crcchkl == 1) {
			dev_err(dev, "Switch reported invalid local CRC on "
				"the uploaded config, retrying...\n");
			continue;
		}
		if (status.crcchkg == 1) {
			dev_err(dev, "Switch reported invalid global CRC on "
				"the uploaded config, retrying...\n");
			continue;
		}
		if (status.configs == 0) {
			dev_err(dev, "Switch reported that configuration is "
				"invalid, retrying...\n");
			continue;
		}
		/* Success! */
		break;
	} while (--retries);

	if (!retries) {
		rc = -EIO;
		dev_err(dev, "Failed to upload config to device, giving up\n");
		goto out;
	} else if (retries != RETRIES) {
		dev_info(dev, "Succeeded after %d tried\n", RETRIES - retries);
	}

	rc = sja1105_ptp_reset(priv);
	if (rc < 0)
		dev_err(dev, "Failed to reset PTP clock: %d\n", rc);

	dev_info(dev, "Reset switch and programmed static config\n");

out:
	kfree(config_buf);
	return rc;
}