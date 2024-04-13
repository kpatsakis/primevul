void TIFF_MetaHandler::CacheFileData()
{
	XMP_IO*      fileRef    = this->parent->ioRef;
	XMP_PacketInfo & packetInfo = this->packetInfo;

	XMP_AbortProc abortProc  = this->parent->abortProc;
	void *        abortArg   = this->parent->abortArg;
	const bool    checkAbort = (abortProc != 0);

	XMP_Assert ( ! this->containsXMP );

	if ( checkAbort && abortProc(abortArg) ) {
		XMP_Throw ( "TIFF_MetaHandler::CacheFileData - User abort", kXMPErr_UserAbort );
	}

	this->tiffMgr.ParseFileStream ( fileRef );

	TIFF_Manager::TagInfo dngInfo;
	if ( this->tiffMgr.GetTag ( kTIFF_PrimaryIFD, kTIFF_DNGVersion, &dngInfo ) ) {


		XMP_Uns8 majorVersion = *((XMP_Uns8*)dngInfo.dataPtr);	// Start with DNGVersion.
		if ( this->tiffMgr.GetTag ( kTIFF_PrimaryIFD, kTIFF_DNGBackwardVersion, &dngInfo ) ) {
			majorVersion = *((XMP_Uns8*)dngInfo.dataPtr);	// Use DNGBackwardVersion if possible.
		}
		if ( majorVersion > 1 ) XMP_Throw ( "DNG version beyond 1.x", kXMPErr_BadTIFF );

	}

	TIFF_Manager::TagInfo xmpInfo;
	bool found = this->tiffMgr.GetTag ( kTIFF_PrimaryIFD, kTIFF_XMP, &xmpInfo );

	if ( found ) {

		this->packetInfo.offset    = this->tiffMgr.GetValueOffset ( kTIFF_PrimaryIFD, kTIFF_XMP );
		this->packetInfo.length    = xmpInfo.dataLen;
		this->packetInfo.padSize   = 0;				// Assume for now, set these properly in ProcessXMP.
		this->packetInfo.charForm  = kXMP_CharUnknown;
		this->packetInfo.writeable = true;

		this->xmpPacket.assign ( (XMP_StringPtr)xmpInfo.dataPtr, xmpInfo.dataLen );

		this->containsXMP = true;

	}

}	// TIFF_MetaHandler::CacheFileData
