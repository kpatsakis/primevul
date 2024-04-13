void TIFF_MetaHandler::UpdateFile ( bool doSafeUpdate )
{
	XMP_Assert ( ! doSafeUpdate );	// This should only be called for "unsafe" updates.

	XMP_IO*   destRef    = this->parent->ioRef;
	XMP_AbortProc abortProc  = this->parent->abortProc;
	void *        abortArg   = this->parent->abortArg;

	XMP_Int64 oldPacketOffset = this->packetInfo.offset;
	XMP_Int32 oldPacketLength = this->packetInfo.length;

	if ( oldPacketOffset == kXMPFiles_UnknownOffset ) oldPacketOffset = 0;	// ! Simplify checks.
	if ( oldPacketLength == kXMPFiles_UnknownLength ) oldPacketLength = 0;

	bool fileHadXMP = ((oldPacketOffset != 0) && (oldPacketLength != 0));


	ExportPhotoData ( kXMP_TIFFFile, &this->xmpObj, &this->tiffMgr, this->iptcMgr, this->psirMgr );

	try {
		XMP_OptionBits options = kXMP_UseCompactFormat;
		if ( fileHadXMP ) options |= kXMP_ExactPacketLength;
		this->xmpObj.SerializeToBuffer ( &this->xmpPacket, options, oldPacketLength );
	} catch ( ... ) {
		this->xmpObj.SerializeToBuffer ( &this->xmpPacket, kXMP_UseCompactFormat );
	}


	bool doInPlace = (fileHadXMP && (this->xmpPacket.size() <= (size_t)oldPacketLength));
	if ( this->tiffMgr.IsLegacyChanged() ) doInPlace = false;
	
	bool localProgressTracking = false;
	XMP_ProgressTracker* progressTracker = this->parent->progressTracker;

	if ( ! doInPlace ) {

		#if GatherPerformanceData
			sAPIPerf->back().extraInfo += ", TIFF append update";
		#endif

		if ( (progressTracker != 0) && (! progressTracker->WorkInProgress()) ) {
			localProgressTracking = true;
			progressTracker->BeginWork();
		}

		this->tiffMgr.SetTag ( kTIFF_PrimaryIFD, kTIFF_XMP, kTIFF_UndefinedType, (XMP_Uns32)this->xmpPacket.size(), this->xmpPacket.c_str() );
		this->tiffMgr.UpdateFileStream ( destRef, progressTracker );

	} else {

		#if GatherPerformanceData
			sAPIPerf->back().extraInfo += ", TIFF in-place update";
		#endif

		if ( this->xmpPacket.size() < (size_t)this->packetInfo.length ) {
			size_t extraSpace = (size_t)this->packetInfo.length - this->xmpPacket.size();
			this->xmpPacket.append ( extraSpace, ' ' );
		}

		XMP_IO* liveFile = this->parent->ioRef;

		XMP_Assert ( this->xmpPacket.size() == (size_t)oldPacketLength );	// ! Done by common PutXMP logic.

		if ( progressTracker != 0 ) {
			if ( progressTracker->WorkInProgress() ) {
				progressTracker->AddTotalWork ( this->xmpPacket.size() );
			} else {
				localProgressTracking = true;
				progressTracker->BeginWork ( this->xmpPacket.size() );
			}
		}

		liveFile->Seek ( oldPacketOffset, kXMP_SeekFromStart  );
		liveFile->Write ( this->xmpPacket.c_str(), (XMP_Int32)this->xmpPacket.size() );

	}
	
	if ( localProgressTracking ) progressTracker->WorkComplete();
	this->needsUpdate = false;

}	// TIFF_MetaHandler::UpdateFile
