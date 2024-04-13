static XMP_Uns16 GetMacScript ( XMP_Uns16 macLang )
{
	XMP_Uns16 macScript = kNoMacScript;
	
	if ( macLang <= 94 ) {
		macScript = kMacLangToScript_0_94[macLang];
	} else if ( (128 <= macLang) && (macLang <= 151) ) {
		macScript = kMacLangToScript_128_151[macLang-128];
	}
	
	return macScript;
	
}	// GetMacScript
