JunkChunk::JunkChunk( ContainerChunk* parent, RIFF_MetaHandler* handler ) : Chunk( parent, handler, true, chunk_JUNK )
{
	chunkType = chunk_JUNK;
}
