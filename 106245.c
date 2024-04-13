XMPChunk::XMPChunk( ContainerChunk* parent, RIFF_MetaHandler* handler ) : Chunk( parent, handler, false, chunk_XMP )
{
	chunkType = chunk_XMP;
	XMP_IO* file = handler->parent->ioRef;
	XMP_Uns8 level = handler->level;

	handler->packetInfo.offset = this->oldPos + 8;
	handler->packetInfo.length = (XMP_Int32) this->oldSize - 8;

	handler->xmpPacket.reserve ( handler->packetInfo.length );
	handler->xmpPacket.assign ( handler->packetInfo.length, ' ' );
	file->ReadAll ( (void*)handler->xmpPacket.data(), handler->packetInfo.length );

	handler->containsXMP = true; // last, after all possible failure

	handler->xmpChunk = this;
}
