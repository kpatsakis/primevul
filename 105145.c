void PSIR_FileWriter::DeleteExistingInfo()
{
	XMP_Assert ( ! (this->memParsed && this->fileParsed) );

	if ( this->memParsed ) {
		if ( this->ownedContent ) free ( this->memContent );
	} else if ( this->fileParsed ) {
		InternalRsrcMap::iterator irPos = this->imgRsrcs.begin();
		InternalRsrcMap::iterator irEnd = this->imgRsrcs.end();
		for ( ; irPos != irEnd; ++irPos ) irPos->second.changed = true;	// Fool the InternalRsrcInfo destructor.
	}

	this->imgRsrcs.clear();

	this->memContent = 0;
	this->memLength  = 0;

	this->changed = false;
	this->legacyDeleted = false;
	this->memParsed = false;
	this->fileParsed = false;
	this->ownedContent = false;

}	// PSIR_FileWriter::DeleteExistingInfo
