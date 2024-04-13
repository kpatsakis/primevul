static XMP_Uns16 GetMacLang ( std::string * xmpLang )
{
	if ( *xmpLang == "" ) return kNoMacLang;
	
	size_t hyphenPos = xmpLang->find ( '-' );	// Make sure the XMP language is "generic".
	if ( hyphenPos != std::string::npos ) xmpLang->erase ( hyphenPos );
	
	for ( XMP_Uns16 i = 0; i <= 94; ++i ) {	// Using std::map would be faster.
		if ( *xmpLang == kMacToXMPLang_0_94[i] ) return i;
	}
	
	for ( XMP_Uns16 i = 128; i <= 151; ++i ) {	// Using std::map would be faster.
		if ( *xmpLang == kMacToXMPLang_128_151[i-128] ) return i;
	}
	
	return kNoMacLang;
	
}	// GetMacLang
