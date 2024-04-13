void JunkChunk::changesAndSize( RIFF_MetaHandler* handler )
{
	this->newSize = this->oldSize; // optimization at a later stage
	XMP_Validate( this->newSize <= 0xFFFFFFFFLL, "no single chunk may be above 4 GB", kXMPErr_InternalFailure );
	if ( this->id == kChunk_JUNQ ) this->hasChange = true;	// Force ID change to JUNK.
}
