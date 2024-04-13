JunkChunk::JunkChunk( ContainerChunk* parent, XMP_Int64 size ) : Chunk( parent, chunk_JUNK, kChunk_JUNK )
{
	XMP_Assert( size >= 8 );
	this->oldSize = size;
	this->newSize = size;
	this->hasChange = true;
}
