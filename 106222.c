bool ConvertFromMacLang ( const std::string & macValue, XMP_Uns16 macLang, std::string * utf8Value )
{
	utf8Value->erase();
	if ( ! IsMacLangKnown ( macLang ) ) return false;
	
	#if XMP_MacBuild
		XMP_Uns16 macScript = GetMacScript ( macLang );
		ReconcileUtils::MacEncodingToUTF8 ( macScript, macLang, (XMP_Uns8*)macValue.c_str(), macValue.size(), utf8Value );
	#elif XMP_UNIXBuild
		MacRomanToUTF8 ( macValue, utf8Value );
	#elif XMP_WinBuild
		UINT winCP = GetWinCP ( macLang );
		ReconcileUtils::WinEncodingToUTF8 ( winCP, (XMP_Uns8*)macValue.c_str(), macValue.size(), utf8Value );
    #elif XMP_iOSBuild
        XMP_Uns32 iosEncCF = GetIOSEncodingCF(macLang);
        ReconcileUtils::IOSConvertEncoding(iosEncCF, kCFStringEncodingUTF8, (XMP_Uns8*)macValue.c_str(), macValue.size(), utf8Value);
#endif
	
	return true;

}	// ConvertFromMacLang
