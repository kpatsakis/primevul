bool TradQT_Manager::ImportSimpleXMP ( XMP_Uns32 id, SXMPMeta * xmp, XMP_StringPtr ns, XMP_StringPtr prop ) const
{

	try {

		InfoMapCPos infoPos = this->parsedBoxes.find ( id );
		if ( infoPos == this->parsedBoxes.end() ) return false;
		if ( infoPos->second.values.empty() ) return false;
		
		std::string xmpValue, tempValue;
		XMP_OptionBits flags;
		bool xmpExists = xmp->GetProperty ( ns, prop, &xmpValue, &flags );
		if ( xmpExists && (! XMP_PropIsSimple ( flags )) ) {
			XMP_Throw ( "TradQT_Manager::ImportSimpleXMP - XMP property must be simple", kXMPErr_BadParam );
		}
		
		bool convertOK;
		const ValueInfo & qtItem = infoPos->second.values[0];	// ! Use the first QT entry.
	
		if ( xmpExists ) {
			convertOK = ConvertToMacLang ( xmpValue, qtItem.macLang, &tempValue );
			if ( ! convertOK ) return false;	// throw?
			if ( tempValue == qtItem.macValue ) return false;	// QT value matches back converted XMP value.
		}
	
		convertOK = ConvertFromMacLang ( qtItem.macValue, qtItem.macLang, &tempValue );
		if ( ! convertOK ) return false;	// throw?
		xmp->SetProperty ( ns, prop, tempValue.c_str() );
		return true;
	
	} catch ( ... ) {

		return false;	// Don't let one failure abort other imports.	
	
	}
		
}	// TradQT_Manager::ImportSimpleXMP
