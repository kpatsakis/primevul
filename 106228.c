static UINT GetWinCP ( XMP_Uns16 macLang )
{
	UINT winCP = 0;
	
	if ( macLang <= 94 ) winCP = kMacToWinCP_0_94[macLang];
	
	if ( winCP == 0 ) {
		XMP_Uns16 macScript = GetMacScript ( macLang );
		if ( macScript != kNoMacScript ) winCP = kMacScriptToWinCP[macScript];
	}
	
	return winCP;
	
}	// GetWinCP
