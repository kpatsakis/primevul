int FAST_FUNC udhcp_str2optset(const char *const_str, void *arg)
{
	struct option_set **opt_list = arg;
	char *opt, *val;
	char *str;
	const struct dhcp_optflag *optflag;
	struct dhcp_optflag bin_optflag;
	unsigned optcode;
	int retval, length;
	/* IP_PAIR needs 8 bytes, STATIC_ROUTES needs 9 max */
	char buffer[9] ALIGNED(4);
	uint16_t *result_u16 = (uint16_t *) buffer;
	uint32_t *result_u32 = (uint32_t *) buffer;

	/* Cheat, the only *const* str possible is "" */
	str = (char *) const_str;
	opt = strtok(str, " \t=");
	if (!opt)
		return 0;

	optcode = bb_strtou(opt, NULL, 0);
	if (!errno && optcode < 255) {
		/* Raw (numeric) option code */
		bin_optflag.flags = OPTION_BIN;
		bin_optflag.code = optcode;
		optflag = &bin_optflag;
	} else {
		optflag = &dhcp_optflags[udhcp_option_idx(opt)];
	}

	retval = 0;
	do {
		val = strtok(NULL, ", \t");
		if (!val)
			break;
		length = dhcp_option_lengths[optflag->flags & OPTION_TYPE_MASK];
		retval = 0;
		opt = buffer; /* new meaning for variable opt */
		switch (optflag->flags & OPTION_TYPE_MASK) {
		case OPTION_IP:
			retval = udhcp_str2nip(val, buffer);
			break;
		case OPTION_IP_PAIR:
			retval = udhcp_str2nip(val, buffer);
			val = strtok(NULL, ", \t/-");
			if (!val)
				retval = 0;
			if (retval)
				retval = udhcp_str2nip(val, buffer + 4);
			break;
		case OPTION_STRING:
		case OPTION_STRING_HOST:
#if ENABLE_FEATURE_UDHCP_RFC3397
		case OPTION_DNS_STRING:
#endif
			length = strnlen(val, 254);
			if (length > 0) {
				opt = val;
				retval = 1;
			}
			break;
		case OPTION_U8:
			buffer[0] = bb_strtou32(val, NULL, 0);
			retval = (errno == 0);
			break;
		/* htonX are macros in older libc's, using temp var
		 * in code below for safety */
		/* TODO: use bb_strtoX? */
		case OPTION_U16: {
			uint32_t tmp = bb_strtou32(val, NULL, 0);
			*result_u16 = htons(tmp);
			retval = (errno == 0 /*&& tmp < 0x10000*/);
			break;
		}
		case OPTION_U32: {
			uint32_t tmp = bb_strtou32(val, NULL, 0);
			*result_u32 = htonl(tmp);
			retval = (errno == 0);
			break;
		}
		case OPTION_S32: {
			int32_t tmp = bb_strtoi32(val, NULL, 0);
			*result_u32 = htonl(tmp);
			retval = (errno == 0);
			break;
		}
		case OPTION_STATIC_ROUTES: {
			/* Input: "a.b.c.d/m" */
			/* Output: mask(1 byte),pfx(0-4 bytes),gw(4 bytes) */
			unsigned mask;
			char *slash = strchr(val, '/');
			if (slash) {
				*slash = '\0';
				retval = udhcp_str2nip(val, buffer + 1);
				buffer[0] = mask = bb_strtou(slash + 1, NULL, 10);
				val = strtok(NULL, ", \t/-");
				if (!val || mask > 32 || errno)
					retval = 0;
				if (retval) {
					length = ((mask + 7) >> 3) + 5;
					retval = udhcp_str2nip(val, buffer + (length - 4));
				}
			}
			break;
		}
		case OPTION_BIN: /* handled in attach_option() */
			opt = val;
			retval = 1;
		default:
			break;
		}
		if (retval)
			attach_option(opt_list, optflag, opt, length);
	} while (retval && (optflag->flags & OPTION_LIST));

	return retval;
}
