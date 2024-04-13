void CSoundFile::FreePattern(LPVOID pat)
{
	if (pat) delete [] (signed char*)pat;
}
