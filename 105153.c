bool PostScript_MetaHandler::ExtractDSCCommentValue(IOBuffer &ioBuf,NativeMetadataIndex index)
{
	
	XMP_IO* fileRef = this->parent->ioRef;
	if ( ! PostScript_Support::SkipTabsAndSpaces(fileRef,ioBuf) ) return false;
	if ( !IsNewline ( *ioBuf.ptr ) )
	{
		do 
		{
			if ( ! CheckFileSpace ( fileRef, &ioBuf, 1 ) ) return false;
			nativeMeta[index] += *ioBuf.ptr;
			++ioBuf.ptr;
		} while ( ! IsNewline ( *ioBuf.ptr) );
		if (!PostScript_Support::HasCodesGT127(nativeMeta[index]))
		{
			dscFlags|=nativeIndextoFlag[index];
		}
		else
			nativeMeta[index].clear();
	}
	return true;
}
