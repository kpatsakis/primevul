bool PostScript_MetaHandler::FindFirstPacket()
{
	int		snipCount;
	bool 	found	= false;
	size_t	bufPos, bufLen;

	XMP_IO* fileRef = this->parent->ioRef;
	XMP_Int64   fileLen = fileRef->Length();
	XMP_PacketInfo & packetInfo = this->packetInfo;

	XMPScanner scanner ( fileLen );
	XMPScanner::SnipInfoVector snips;

	enum { kBufferSize = 64*1024 };
	XMP_Uns8	buffer [kBufferSize];

	XMP_AbortProc abortProc  = this->parent->abortProc;
	void *        abortArg   = this->parent->abortArg;
	const bool    checkAbort = (abortProc != 0);

	bufPos = 0;
	bufLen = 0;

	fileRef->Rewind();	// Seek back to the beginning of the file.
	bool firstfound=false;

	while ( true ) 
	{

		if ( checkAbort && abortProc(abortArg) ) 
		{
			XMP_Throw ( "PostScript_MetaHandler::FindFirstPacket - User abort", kXMPErr_UserAbort );
		}

		bufPos += bufLen;
		bufLen = fileRef->Read ( buffer, kBufferSize );
		if ( bufLen == 0 ) return firstfound;	// Must be at EoF, no packets found.

		scanner.Scan ( buffer, bufPos, bufLen );
		snipCount = scanner.GetSnipCount();
		scanner.Report ( snips );
		for ( int i = 0; i < snipCount; ++i ) 
		{
			if ( snips[i].fState == XMPScanner::eValidPacketSnip ) 
			{
				if (!firstfound)
				{
					if ( snips[i].fLength > 0x7FFFFFFF ) XMP_Throw ( "PostScript_MetaHandler::FindFirstPacket: Oversize packet", kXMPErr_BadXMP );
					packetInfo.offset = snips[i].fOffset;
					packetInfo.length = (XMP_Int32)snips[i].fLength;
					packetInfo.charForm  = snips[i].fCharForm;
					packetInfo.writeable = (snips[i].fAccess == 'w');
					firstPacketInfo=packetInfo;
					lastPacketInfo=packetInfo;
					firstfound=true;
				}
				else
				{					
					lastPacketInfo.offset = snips[i].fOffset;
					lastPacketInfo.length = (XMP_Int32)snips[i].fLength;
					lastPacketInfo.charForm  = snips[i].fCharForm;
					lastPacketInfo.writeable = (snips[i].fAccess == 'w');
				}
			}
		}

	}
	
	return firstfound;

}	// FindFirstPacket
