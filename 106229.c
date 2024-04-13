static XMP_StringPtr GetXMPLang ( XMP_Uns16 macLang )
{
	XMP_StringPtr xmpLang = "";
	
	if ( macLang <= 94 ) {
		xmpLang = kMacToXMPLang_0_94[macLang];
	} else if ( (128 <= macLang) && (macLang <= 151) ) {
		xmpLang = kMacToXMPLang_128_151[macLang-128];
	}
	
	return xmpLang;
	
}	// GetXMPLang
