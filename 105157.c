int PostScript_MetaHandler::FindPostScriptHint()
{
	bool     found = false;
	IOBuffer ioBuf;
	XMP_Uns8 ch;

	XMP_IO* fileRef = this->parent->ioRef;

	XMP_AbortProc abortProc  = this->parent->abortProc;
	void *        abortArg   = this->parent->abortArg;
	const bool    checkAbort = (abortProc != 0);


	fileRef->Rewind();
	if ( ! CheckFileSpace ( fileRef, &ioBuf, 4 ) ) return false;
	XMP_Uns32 fileheader = GetUns32BE ( ioBuf.ptr );

	if ( fileheader == 0xC5D0D3C6 ) {

		if ( ! CheckFileSpace ( fileRef, &ioBuf, 30 ) ) return false;

		XMP_Uns32 psOffset = GetUns32LE ( ioBuf.ptr+4 );	// PostScript offset.
		XMP_Uns32 psLength = GetUns32LE ( ioBuf.ptr+8 );	// PostScript length.

		MoveToOffset ( fileRef, psOffset, &ioBuf );

	}


	while ( true ) {

		if ( checkAbort && abortProc(abortArg) ) {
			XMP_Throw ( "PostScript_MetaHandler::FindPostScriptHint - User abort", kXMPErr_UserAbort );
		}

		if ( ! CheckFileSpace ( fileRef, &ioBuf, kPSContainsXMPString.length() ) ) return kPSHint_NoMarker;

		if ( CheckBytes ( ioBuf.ptr, Uns8Ptr(kPSEndCommentString.c_str()), kPSEndCommentString.length() ) ) {

			return kPSHint_NoMarker;

		} else if ( ! CheckBytes ( ioBuf.ptr, Uns8Ptr(kPSContainsXMPString.c_str()), kPSContainsXMPString.length() ) ) {

			do {
				if ( ! CheckFileSpace ( fileRef, &ioBuf, 1 ) ) return kPSHint_NoMarker;
				ch = *ioBuf.ptr;
				++ioBuf.ptr;
			} while ( ! IsNewline ( ch ) );

		} else {


			ioBuf.ptr += kPSContainsXMPString.length();
			int xmpHint = kPSHint_NoMain;	// ! From here on, a failure means "no main", not "no marker".
			if ( ! CheckFileSpace ( fileRef, &ioBuf, 1 ) ) return kPSHint_NoMain;
			if ( ! IsSpaceOrTab ( *ioBuf.ptr ) ) return kPSHint_NoMain;

			while ( true ) {

				while ( true ) {	// Skip leading spaces and tabs.
					if ( ! CheckFileSpace ( fileRef, &ioBuf, 1 ) ) return kPSHint_NoMain;
					if ( ! IsSpaceOrTab ( *ioBuf.ptr ) ) break;
					++ioBuf.ptr;
				}
				if ( IsNewline ( *ioBuf.ptr ) ) return kPSHint_NoMain;	// Reached the end of the ContainsXMP comment.

				if ( ! CheckFileSpace ( fileRef, &ioBuf, 6 ) ) return kPSHint_NoMain;

				if ( CheckBytes ( ioBuf.ptr, Uns8Ptr("NoMain"), 6 ) ) {

					ioBuf.ptr += 6;
					xmpHint = kPSHint_NoMain;
					break;

				} else if ( CheckBytes ( ioBuf.ptr, Uns8Ptr("MainFi"), 6 ) ) {

					ioBuf.ptr += 6;
					if ( ! CheckFileSpace ( fileRef, &ioBuf, 3 ) ) return kPSHint_NoMain;
					if ( CheckBytes ( ioBuf.ptr, Uns8Ptr("rst"), 3 ) ) {
						ioBuf.ptr += 3;
						xmpHint = kPSHint_MainFirst;
					}
					break;

				} else if ( CheckBytes ( ioBuf.ptr, Uns8Ptr("MainLa"), 6 ) ) {

					ioBuf.ptr += 6;
					if ( ! CheckFileSpace ( fileRef, &ioBuf, 2 ) ) return kPSHint_NoMain;
					if ( CheckBytes ( ioBuf.ptr, Uns8Ptr("st"), 2 ) ) {
						ioBuf.ptr += 2;
						xmpHint = kPSHint_MainLast;
					}
					break;

				} else {

					while ( true ) {	// Skip until whitespace.
						if ( ! CheckFileSpace ( fileRef, &ioBuf, 1 ) ) return kPSHint_NoMain;
						if ( IsWhitespace ( *ioBuf.ptr ) ) break;
						++ioBuf.ptr;
					}

				}

			}	// Look for the main packet location option.

			if ( ! CheckFileSpace ( fileRef, &ioBuf, 1 ) ) return kPSHint_NoMain;
			if ( ! IsWhitespace ( *ioBuf.ptr ) ) return kPSHint_NoMain;
			return xmpHint;

		}	// Found "%ADO_ContainsXMP:".

	}	// Outer marker loop.

	return kPSHint_NoMarker;	// Should never reach here.

}	// PostScript_MetaHandler::FindPostScriptHint
