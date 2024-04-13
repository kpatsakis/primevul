read_eeprom (long ioaddr, int eep_addr)
{
	int i = 1000;
	outw (EEP_READ | (eep_addr & 0xff), ioaddr + EepromCtrl);
	while (i-- > 0) {
		if (!(inw (ioaddr + EepromCtrl) & EEP_BUSY)) {
			return inw (ioaddr + EepromData);
		}
	}
	return 0;
}
