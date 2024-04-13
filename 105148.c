static inline bool IsMetadataImgRsrc ( XMP_Uns16 id )
{
	if ( id == 0 ) return false;

	int i;
	for ( i = 0; id < kPSIR_MetadataIDs[i]; ++i ) {}
	if ( id == kPSIR_MetadataIDs[i] ) return true;
	return false;

}	// IsMetadataImgRsrc
