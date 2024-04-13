des_setkey(const char *key)
{
	uint32		k0,
				k1,
				rawkey0,
				rawkey1;
	int			shifts,
				round;

	if (!des_initialised)
		des_init();

	rawkey0 = ntohl(*(const uint32 *) key);
	rawkey1 = ntohl(*(const uint32 *) (key + 4));

	if ((rawkey0 | rawkey1)
		&& rawkey0 == old_rawkey0
		&& rawkey1 == old_rawkey1)
	{
		/*
		 * Already setup for this key. This optimisation fails on a zero key
		 * (which is weak and has bad parity anyway) in order to simplify the
		 * starting conditions.
		 */
		return (0);
	}
	old_rawkey0 = rawkey0;
	old_rawkey1 = rawkey1;

	/*
	 * Do key permutation and split into two 28-bit subkeys.
	 */
	k0 = key_perm_maskl[0][rawkey0 >> 25]
		| key_perm_maskl[1][(rawkey0 >> 17) & 0x7f]
		| key_perm_maskl[2][(rawkey0 >> 9) & 0x7f]
		| key_perm_maskl[3][(rawkey0 >> 1) & 0x7f]
		| key_perm_maskl[4][rawkey1 >> 25]
		| key_perm_maskl[5][(rawkey1 >> 17) & 0x7f]
		| key_perm_maskl[6][(rawkey1 >> 9) & 0x7f]
		| key_perm_maskl[7][(rawkey1 >> 1) & 0x7f];
	k1 = key_perm_maskr[0][rawkey0 >> 25]
		| key_perm_maskr[1][(rawkey0 >> 17) & 0x7f]
		| key_perm_maskr[2][(rawkey0 >> 9) & 0x7f]
		| key_perm_maskr[3][(rawkey0 >> 1) & 0x7f]
		| key_perm_maskr[4][rawkey1 >> 25]
		| key_perm_maskr[5][(rawkey1 >> 17) & 0x7f]
		| key_perm_maskr[6][(rawkey1 >> 9) & 0x7f]
		| key_perm_maskr[7][(rawkey1 >> 1) & 0x7f];

	/*
	 * Rotate subkeys and do compression permutation.
	 */
	shifts = 0;
	for (round = 0; round < 16; round++)
	{
		uint32		t0,
					t1;

		shifts += key_shifts[round];

		t0 = (k0 << shifts) | (k0 >> (28 - shifts));
		t1 = (k1 << shifts) | (k1 >> (28 - shifts));

		de_keysl[15 - round] =
			en_keysl[round] = comp_maskl[0][(t0 >> 21) & 0x7f]
			| comp_maskl[1][(t0 >> 14) & 0x7f]
			| comp_maskl[2][(t0 >> 7) & 0x7f]
			| comp_maskl[3][t0 & 0x7f]
			| comp_maskl[4][(t1 >> 21) & 0x7f]
			| comp_maskl[5][(t1 >> 14) & 0x7f]
			| comp_maskl[6][(t1 >> 7) & 0x7f]
			| comp_maskl[7][t1 & 0x7f];

		de_keysr[15 - round] =
			en_keysr[round] = comp_maskr[0][(t0 >> 21) & 0x7f]
			| comp_maskr[1][(t0 >> 14) & 0x7f]
			| comp_maskr[2][(t0 >> 7) & 0x7f]
			| comp_maskr[3][t0 & 0x7f]
			| comp_maskr[4][(t1 >> 21) & 0x7f]
			| comp_maskr[5][(t1 >> 14) & 0x7f]
			| comp_maskr[6][(t1 >> 7) & 0x7f]
			| comp_maskr[7][t1 & 0x7f];
	}
	return (0);
}
