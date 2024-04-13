bool PostScript_MetaHandler::FindLastPacket()
{
	size_t	bufPos, bufLen;

	XMP_IO* fileRef = this->parent->ioRef;
	XMP_Int64   fileLen = fileRef->Length();
	XMP_PacketInfo & packetInfo = this->packetInfo;


	XMPScanner	scanner ( fileLen );

	enum { kBufferSize = 64*1024 };
	XMP_Uns8	buffer [kBufferSize];

	XMP_AbortProc abortProc  = this->parent->abortProc;
	void *        abortArg   = this->parent->abortArg;
	const bool    checkAbort = (abortProc != 0);

	fileRef->Rewind();	// Seek back to the beginning of the file.

	for ( bufPos = 0; bufPos < (size_t)fileLen; bufPos += bufLen )
	{
		if ( checkAbort && abortProc(abortArg) ) 
		{
			XMP_Throw ( "PostScript_MetaHandler::FindLastPacket - User abort", kXMPErr_UserAbort );
		}
		bufLen = fileRef->Read ( buffer, kBufferSize );
		if ( bufLen == 0 ) XMP_Throw ( "PostScript_MetaHandler::FindLastPacket: Read failure", kXMPErr_ExternalFailure );
		scanner.Scan ( buffer, bufPos, bufLen );
	}


	int snipCount = scanner.GetSnipCount();

	XMPScanner::SnipInfoVector snips ( snipCount );
	scanner.Report ( snips );

	bool lastfound=false;
	for ( int i = 0; i < snipCount; ++i ) 
	{
		if ( snips[i].fState == XMPScanner::eValidPacketSnip ) 
		{
			if (!lastfound)
			{
				if ( snips[i].fLength > 0x7FFFFFFF ) XMP_Throw ( "PostScript_MetaHandler::FindLastPacket: Oversize packet", kXMPErr_BadXMP );
				packetInfo.offset = snips[i].fOffset;
				packetInfo.length = (XMP_Int32)snips[i].fLength;
				packetInfo.charForm  = snips[i].fCharForm;
				packetInfo.writeable = (snips[i].fAccess == 'w');
				firstPacketInfo=packetInfo;
				lastPacketInfo=packetInfo;
				lastfound=true;
			}
			else
			{					
				lastPacketInfo.offset = snips[i].fOffset;
				lastPacketInfo.length = (XMP_Int32)snips[i].fLength;
				lastPacketInfo.charForm  = snips[i].fCharForm;
				lastPacketInfo.writeable = (snips[i].fAccess == 'w');
				packetInfo=lastPacketInfo;
			}
		}
	}
	return lastfound;

}	// PostScript_MetaHandler::FindLastPacket
