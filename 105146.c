bool PSIR_FileWriter::GetImgRsrc ( XMP_Uns16 id, ImgRsrcInfo* info ) const
{
	InternalRsrcMap::const_iterator rsrcPos = this->imgRsrcs.find ( id );
	if ( rsrcPos == this->imgRsrcs.end() ) return false;

	const InternalRsrcInfo & rsrcInfo = rsrcPos->second;

	if ( info != 0 ) {
		info->id = rsrcInfo.id;
		info->dataLen = rsrcInfo.dataLen;
		info->dataPtr = rsrcInfo.dataPtr;
		info->origOffset = rsrcInfo.origOffset;
	}

	return true;

}	// PSIR_FileWriter::GetImgRsrc
