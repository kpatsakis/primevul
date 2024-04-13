static void print_section(char *text, u8 *addr, unsigned int length)
{
	int i, offset;
	int newline = 1;
	char ascii[17];

	ascii[16] = 0;

	for (i = 0; i < length; i++) {
		if (newline) {
			printk(KERN_ERR "%8s 0x%p: ", text, addr + i);
			newline = 0;
		}
		printk(KERN_CONT " %02x", addr[i]);
		offset = i % 16;
		ascii[offset] = isgraph(addr[i]) ? addr[i] : '.';
		if (offset == 15) {
			printk(KERN_CONT " %s\n", ascii);
			newline = 1;
		}
	}
	if (!newline) {
		i %= 16;
		while (i < 16) {
			printk(KERN_CONT "   ");
			ascii[i] = ' ';
			i++;
		}
		printk(KERN_CONT " %s\n", ascii);
	}
}
