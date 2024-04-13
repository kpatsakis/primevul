PSIR_FileWriter::~PSIR_FileWriter()
{
	XMP_Assert ( ! (this->memParsed && this->fileParsed) );

	if ( this->ownedContent ) {
		XMP_Assert ( this->memContent != 0 );
		free ( this->memContent );
	}

}	// PSIR_FileWriter::~PSIR_FileWriter
