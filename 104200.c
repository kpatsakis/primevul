get_standard_integer_type(const char *l, const char **t)
{
	int type;

	if (isalpha((unsigned char)l[1])) {
		switch (l[1]) {
		case 'C':
			/* "dC" and "uC" */
			type = FILE_BYTE;
			break;
		case 'S':
			/* "dS" and "uS" */
			type = FILE_SHORT;
			break;
		case 'I':
		case 'L':
			/*
			 * "dI", "dL", "uI", and "uL".
			 *
			 * XXX - the actual Single UNIX Specification says
			 * that "L" means "long", as in the C data type,
			 * but we treat it as meaning "4-byte integer".
			 * Given that the OS X version of file 5.04 did
			 * the same, I guess that passes the actual SUS
			 * validation suite; having "dL" be dependent on
			 * how big a "long" is on the machine running
			 * "file" is silly.
			 */
			type = FILE_LONG;
			break;
		case 'Q':
			/* "dQ" and "uQ" */
			type = FILE_QUAD;
			break;
		default:
			/* "d{anything else}", "u{anything else}" */
			return FILE_INVALID;
		}
		l += 2;
	} else if (isdigit((unsigned char)l[1])) {
		/*
		 * "d{num}" and "u{num}"; we only support {num} values
		 * of 1, 2, 4, and 8 - the Single UNIX Specification
		 * doesn't say anything about whether arbitrary
		 * values should be supported, but both the Solaris 10
		 * and OS X Mountain Lion versions of file passed the
		 * Single UNIX Specification validation suite, and
		 * neither of them support values bigger than 8 or
		 * non-power-of-2 values.
		 */
		if (isdigit((unsigned char)l[2])) {
			/* Multi-digit, so > 9 */
			return FILE_INVALID;
		}
		switch (l[1]) {
		case '1':
			type = FILE_BYTE;
			break;
		case '2':
			type = FILE_SHORT;
			break;
		case '4':
			type = FILE_LONG;
			break;
		case '8':
			type = FILE_QUAD;
			break;
		default:
			/* XXX - what about 3, 5, 6, or 7? */
			return FILE_INVALID;
		}
		l += 2;
	} else {
		/*
		 * "d" or "u" by itself.
		 */
		type = FILE_LONG;
		++l;
	}
	if (t)
		*t = l;
	return type;
}
