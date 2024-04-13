void TradQT_Manager::ExportSimpleXMP ( XMP_Uns32 id, const SXMPMeta & xmp, XMP_StringPtr ns, XMP_StringPtr prop,
									   bool createWithZeroLang /* = false */  )
{
	std::string xmpValue, macValue;

	InfoMapPos infoPos = this->parsedBoxes.find ( id );
	bool qtFound = (infoPos != this->parsedBoxes.end()) && (! infoPos->second.values.empty());

	bool xmpFound = xmp.GetProperty ( ns, prop, &xmpValue, 0 );
	if ( (! xmpFound) || (xmpValue.empty()) ) {
		if ( qtFound ) {
			this->parsedBoxes.erase ( infoPos );
			this->changed = true;
		}
		return;
	}
	
	XMP_Assert ( xmpFound );
	if ( ! qtFound ) {
		if ( ! createWithZeroLang ) return;
		infoPos = this->parsedBoxes.insert ( this->parsedBoxes.end(),
											 InfoMap::value_type ( id, ParsedBoxInfo ( id ) ) );
		ValueVector * newValues = &infoPos->second.values;
		newValues->push_back ( ValueInfo() );
		ValueInfo * newValue = &newValues->back();
		newValue->macLang = 0;	// Happens to be langEnglish.
		newValue->xmpLang = kMacToXMPLang_0_94[0];
		this->changed = infoPos->second.changed = true;
	}
		
	ValueInfo * qtItem = &infoPos->second.values[0];	// ! Use the first QT entry.
	if ( ! IsMacLangKnown ( qtItem->macLang ) ) return;
	
	bool convertOK = ConvertToMacLang ( xmpValue, qtItem->macLang, &macValue );
	if ( convertOK && (macValue != qtItem->macValue) ) {
		qtItem->macValue = macValue;
		this->changed = infoPos->second.changed = true;
	}
	
}	// TradQT_Manager::ExportSimpleXMP
