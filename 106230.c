bool TradQT_Manager::ImportLangAltXMP ( XMP_Uns32 id, SXMPMeta * xmp, XMP_StringPtr ns, XMP_StringPtr langArray ) const
{

	try {

		InfoMapCPos infoPos = this->parsedBoxes.find ( id );
		if ( infoPos == this->parsedBoxes.end() ) return false;
		if ( infoPos->second.values.empty() ) return false;	// Quit now if there are no values.
		
		XMP_OptionBits flags;
		bool xmpExists = xmp->GetProperty ( ns, langArray, 0, &flags );
		if ( ! xmpExists ) {
			xmp->SetProperty ( ns, langArray, 0, kXMP_PropArrayIsAltText );
		} else if ( ! XMP_ArrayIsAltText ( flags ) ) {
			XMP_Throw ( "TradQT_Manager::ImportLangAltXMP - XMP array must be AltText", kXMPErr_BadParam );
		}
	
		
		bool haveMappings = false;
		const ValueVector & qtValues = infoPos->second.values;
		
		for ( size_t i = 0, limit = qtValues.size(); i < limit; ++i ) {
			const ValueInfo & qtItem = qtValues[i];
			if ( *qtItem.xmpLang == 0 ) continue;	// Only do known mappings in the loop.
			haveMappings |= this->ImportLangItem ( qtItem, xmp, ns, langArray );
		}
		
		if ( ! haveMappings ) {
			haveMappings = this->ImportLangItem ( qtValues[0], xmp, ns, langArray );	// ! No xmpLang implies "x-default".
		}
	
		return haveMappings;
	
	} catch ( ... ) {

		return false;	// Don't let one failure abort other imports.	
	
	}

}	// TradQT_Manager::ImportLangAltXMP
