static inline struct radeon_gpio_rec radeon_lookup_gpio(struct radeon_device *rdev,
							u8 id)
{
	struct atom_context *ctx = rdev->mode_info.atom_context;
	struct radeon_gpio_rec gpio;
	int index = GetIndexIntoMasterTable(DATA, GPIO_Pin_LUT);
	struct _ATOM_GPIO_PIN_LUT *gpio_info;
	ATOM_GPIO_PIN_ASSIGNMENT *pin;
	u16 data_offset, size;
	int i, num_indices;

	memset(&gpio, 0, sizeof(struct radeon_gpio_rec));
	gpio.valid = false;

	if (atom_parse_data_header(ctx, index, &size, NULL, NULL, &data_offset)) {
		gpio_info = (struct _ATOM_GPIO_PIN_LUT *)(ctx->bios + data_offset);

		num_indices = (size - sizeof(ATOM_COMMON_TABLE_HEADER)) /
			sizeof(ATOM_GPIO_PIN_ASSIGNMENT);

		for (i = 0; i < num_indices; i++) {
			pin = &gpio_info->asGPIO_Pin[i];
			if (id == pin->ucGPIO_ID) {
				gpio.id = pin->ucGPIO_ID;
				gpio.reg = pin->usGpioPin_AIndex * 4;
				gpio.mask = (1 << pin->ucGpioPinBitShift);
				gpio.valid = true;
				break;
			}
		}
	}

	return gpio;
}
