bool ConvertToMacLang ( const std::string & utf8Value, XMP_Uns16 macLang, std::string * macValue )
{
	macValue->erase();
	if ( macLang == kNoMacLang ) macLang = 0;	// *** Zero is English, ought to use the "active" OS lang.
	if ( ! IsMacLangKnown ( macLang ) ) return false;

	#if XMP_MacBuild
		XMP_Uns16 macScript = GetMacScript ( macLang );
		ReconcileUtils::UTF8ToMacEncoding ( macScript, macLang, (XMP_Uns8*)utf8Value.c_str(), utf8Value.size(), macValue );
	#elif XMP_UNIXBuild
		UTF8ToMacRoman ( utf8Value, macValue );
	#elif XMP_WinBuild
		UINT winCP = GetWinCP ( macLang );
		ReconcileUtils::UTF8ToWinEncoding ( winCP, (XMP_Uns8*)utf8Value.c_str(), utf8Value.size(), macValue );
    #elif XMP_iOSBuild
        XMP_Uns32 iosEncCF = GetIOSEncodingCF(macLang);
        ReconcileUtils::IOSConvertEncoding(kCFStringEncodingUTF8, iosEncCF, (XMP_Uns8*)utf8Value.c_str(), utf8Value.size(), macValue);
    #endif
	
	return true;

}	// ConvertToMacLang
