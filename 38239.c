static int winbpp (unsigned int winctrl1)
{
	int bits = 0;

	/* how many bits are needed for each pixel format */
	switch (winctrl1 & LCD_WINCTRL1_FRM) {
	case LCD_WINCTRL1_FRM_1BPP:
		bits = 1;
		break;
	case LCD_WINCTRL1_FRM_2BPP:
		bits = 2;
		break;
	case LCD_WINCTRL1_FRM_4BPP:
		bits = 4;
		break;
	case LCD_WINCTRL1_FRM_8BPP:
		bits = 8;
		break;
	case LCD_WINCTRL1_FRM_12BPP:
	case LCD_WINCTRL1_FRM_16BPP655:
	case LCD_WINCTRL1_FRM_16BPP565:
	case LCD_WINCTRL1_FRM_16BPP556:
	case LCD_WINCTRL1_FRM_16BPPI1555:
	case LCD_WINCTRL1_FRM_16BPPI5551:
	case LCD_WINCTRL1_FRM_16BPPA1555:
	case LCD_WINCTRL1_FRM_16BPPA5551:
		bits = 16;
		break;
	case LCD_WINCTRL1_FRM_24BPP:
	case LCD_WINCTRL1_FRM_32BPP:
		bits = 32;
		break;
	}

	return bits;
}
