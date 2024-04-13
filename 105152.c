bool PostScript_MetaHandler::ExtractContainsXMPHint(IOBuffer &ioBuf,XMP_Int64 containsXMPStartpos)
{
	XMP_IO* fileRef = this->parent->ioRef;
	int xmpHint = kPSHint_NoMain;	// ! From here on, a failure means "no main", not "no marker".
	if ( ! CheckFileSpace ( fileRef, &ioBuf, 1 ) ) return false;
	if ( ! IsSpaceOrTab ( *ioBuf.ptr ) ) return false;
	if ( ! PostScript_Support::SkipTabsAndSpaces(fileRef,ioBuf) ) return false;
	if ( IsNewline ( *ioBuf.ptr ) ) return false;	// Reached the end of the ContainsXMP comment.

	if ( ! CheckFileSpace ( fileRef, &ioBuf, 6 ) ) return false ;

	if ( CheckBytes ( ioBuf.ptr, Uns8Ptr("NoMain"), 6 ) ) 
	{
		ioBuf.ptr += 6;
		if ( ! PostScript_Support::SkipTabsAndSpaces(fileRef,ioBuf) ) return false;
		if ( ! IsNewline( *ioBuf.ptr) ) return false;
		this->psHint = kPSHint_NoMain;
		setTokenInfo(kPS_ADOContainsXMP,containsXMPStartpos,ioBuf.filePos+ioBuf.ptr-ioBuf.data-containsXMPStartpos);

	} 
	else if ( CheckBytes ( ioBuf.ptr, Uns8Ptr("MainFi"), 6 ) ) 
	{
		ioBuf.ptr += 6;
		if ( ! CheckFileSpace ( fileRef, &ioBuf, 3 ) ) return false;
		if ( CheckBytes ( ioBuf.ptr, Uns8Ptr("rst"), 3 ) ) 
		{
			ioBuf.ptr += 3;
			if ( ! PostScript_Support::SkipTabsAndSpaces(fileRef,ioBuf) ) return false;
			if ( ! IsNewline( *ioBuf.ptr) ) return false;
			this->psHint = kPSHint_MainFirst;
			setTokenInfo(kPS_ADOContainsXMP,containsXMPStartpos,ioBuf.filePos+ioBuf.ptr-ioBuf.data-containsXMPStartpos);
			containsXMPHint=true;
		}
	} 
	else if ( CheckBytes ( ioBuf.ptr, Uns8Ptr("MainLa"), 6 ) ) 
	{
		ioBuf.ptr += 6;
		if ( ! CheckFileSpace ( fileRef, &ioBuf, 2 ) ) return false;
		if ( CheckBytes ( ioBuf.ptr, Uns8Ptr("st"), 2 ) ) {
			ioBuf.ptr += 2;
			if ( ! PostScript_Support::SkipTabsAndSpaces(fileRef,ioBuf) ) return false;
			if ( ! IsNewline( *ioBuf.ptr) ) return false;
			this->psHint = kPSHint_MainLast;
			setTokenInfo(kPS_ADOContainsXMP,containsXMPStartpos,ioBuf.filePos+ioBuf.ptr-ioBuf.data-containsXMPStartpos);
			containsXMPHint=true;
		}
	} 
	else 
	{
		if ( ! PostScript_Support::SkipUntilNewline(fileRef,ioBuf) ) return false;
	}
	return true;
}
