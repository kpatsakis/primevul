bool PSIR_FileWriter::IsLegacyChanged()
{

	if ( ! this->changed ) return false;
	if ( this->legacyDeleted ) return true;

	InternalRsrcMap::iterator irPos = this->imgRsrcs.begin();
	InternalRsrcMap::iterator irEnd = this->imgRsrcs.end();

	for ( ; irPos != irEnd; ++irPos ) {
		const InternalRsrcInfo & rsrcInfo = irPos->second;
		if ( rsrcInfo.changed && (rsrcInfo.id != kPSIR_XMP) ) return true;
	}

	return false;	// Can get here if the XMP is the only thing changed.

}	// PSIR_FileWriter::IsLegacyChanged
