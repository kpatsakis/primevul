static inline bool IsMacLangKnown ( XMP_Uns16 macLang )
{
	XMP_Uns16 macScript = GetMacScript ( macLang );
	if ( macScript == kNoMacScript ) return false;

	#if XMP_UNIXBuild
		if ( macScript != smRoman ) return false;
	#elif XMP_WinBuild
		if ( GetWinCP(macLang) == 0 ) return false;
	#endif
	
	return true;

}	// IsMacLangKnown
