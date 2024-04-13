parse_eeprom (struct net_device *dev)
{
	int i, j;
	long ioaddr = dev->base_addr;
	u8 sromdata[256];
	u8 *psib;
	u32 crc;
	PSROM_t psrom = (PSROM_t) sromdata;
	struct netdev_private *np = netdev_priv(dev);

	int cid, next;

#ifdef	MEM_MAPPING
	ioaddr = pci_resource_start (np->pdev, 0);
#endif
	/* Read eeprom */
	for (i = 0; i < 128; i++) {
		((__le16 *) sromdata)[i] = cpu_to_le16(read_eeprom (ioaddr, i));
	}
#ifdef	MEM_MAPPING
	ioaddr = dev->base_addr;
#endif
	if (np->pdev->vendor == PCI_VENDOR_ID_DLINK) {	/* D-Link Only */
		/* Check CRC */
		crc = ~ether_crc_le (256 - 4, sromdata);
		if (psrom->crc != cpu_to_le32(crc)) {
			printk (KERN_ERR "%s: EEPROM data CRC error.\n",
					dev->name);
			return -1;
		}
	}

	/* Set MAC address */
	for (i = 0; i < 6; i++)
		dev->dev_addr[i] = psrom->mac_addr[i];

	if (np->pdev->vendor != PCI_VENDOR_ID_DLINK) {
		return 0;
	}

	/* Parse Software Information Block */
	i = 0x30;
	psib = (u8 *) sromdata;
	do {
		cid = psib[i++];
		next = psib[i++];
		if ((cid == 0 && next == 0) || (cid == 0xff && next == 0xff)) {
			printk (KERN_ERR "Cell data error\n");
			return -1;
		}
		switch (cid) {
		case 0:	/* Format version */
			break;
		case 1:	/* End of cell */
			return 0;
		case 2:	/* Duplex Polarity */
			np->duplex_polarity = psib[i];
			writeb (readb (ioaddr + PhyCtrl) | psib[i],
				ioaddr + PhyCtrl);
			break;
		case 3:	/* Wake Polarity */
			np->wake_polarity = psib[i];
			break;
		case 9:	/* Adapter description */
			j = (next - i > 255) ? 255 : next - i;
			memcpy (np->name, &(psib[i]), j);
			break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:	/* Reversed */
			break;
		default:	/* Unknown cell */
			return -1;
		}
		i = next;
	} while (1);

	return 0;
}
