int CSoundFile::FrequencyToTranspose(DWORD freq)
{
	return int(1536*(log(freq/8363.0)/log(2.0)));

#ifdef MSC_VER
	const float _f1_8363 = 1.0f / 8363.0f;
	const float _factor = 128 * 12;
	LONG result;

	if (!freq) return 0;
	_asm {
	fld _factor
	fild freq
	fld _f1_8363
	fmulp st(1), st(0)
	fyl2x
	fistp result
	}
	return result;
#endif
}
