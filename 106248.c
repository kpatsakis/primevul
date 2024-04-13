void XMPChunk::changesAndSize( RIFF_MetaHandler* handler )
{
	XMP_Enforce( &handler->xmpPacket != 0 );
	XMP_Enforce( handler->xmpPacket.size() > 0 );
	this->newSize = 8 + handler->xmpPacket.size();

	XMP_Validate( this->newSize <= 0xFFFFFFFFLL, "no single chunk may be above 4 GB", kXMPErr_InternalFailure );

	this->hasChange = true;
}
