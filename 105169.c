void TIFF_MetaHandler::WriteTempFile ( XMP_IO* tempRef )
{
	XMP_IO* origRef = this->parent->ioRef;

	XMP_AbortProc abortProc  = this->parent->abortProc;
	void *        abortArg   = this->parent->abortArg;

	XMP_Int64 fileLen = origRef->Length();
	if ( fileLen > 0xFFFFFFFFLL ) {	// Check before making a copy of the file.
		XMP_Throw ( "TIFF fles can't exceed 4GB", kXMPErr_BadTIFF );
	}
	
	XMP_ProgressTracker* progressTracker = this->parent->progressTracker;
	if ( progressTracker != 0 ) progressTracker->BeginWork ( (float)fileLen );

	origRef->Rewind ( );
	tempRef->Truncate ( 0 );
	XIO::Copy ( origRef, tempRef, fileLen, abortProc, abortArg );

	try {
		this->parent->ioRef = tempRef;	// ! Make UpdateFile update the temp.
		this->UpdateFile ( false );
		this->parent->ioRef = origRef;
	} catch ( ... ) {
		this->parent->ioRef = origRef;
		throw;
	}
	
	if ( progressTracker != 0 ) progressTracker->WorkComplete();

}	// TIFF_MetaHandler::WriteTempFile
