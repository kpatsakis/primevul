UINT CSoundFile::GetInstrumentName(UINT nInstr,LPSTR s) const
{
        char sztmp[40] = "";  // changed from CHAR
	if ((nInstr >= MAX_INSTRUMENTS) || (!Headers[nInstr]))
	{
		if (s) *s = 0;
		return 0;
	}
	INSTRUMENTHEADER *penv = Headers[nInstr];
	memcpy(sztmp, penv->name, 32);
	sztmp[31] = 0;
	if (s) strcpy(s, sztmp);
	return strlen(sztmp);
}
