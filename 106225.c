static XMP_Uns32 GetIOSEncodingCF ( XMP_Uns16 macLang )
{
	XMP_Uns32 encCF = kCFStringEncodingInvalidId;
	
	if ( macLang <= 94 ) encCF = kMacToIOSEncodingCF_0_94[macLang];
	
	if ( encCF == kCFStringEncodingInvalidId || !CFStringIsEncodingAvailable(encCF)) {
		XMP_Uns16 macScript = GetMacScript ( macLang );
		if ( macScript != kNoMacScript ) encCF = kMacScriptToIOSEncodingCF[macScript];
	}
	
	return encCF;
	
}	// GetIOSEncodingCF
