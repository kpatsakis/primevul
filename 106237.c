ContainerChunk::ContainerChunk( ContainerChunk* parent, XMP_Uns32 id, XMP_Uns32 containerType ) : Chunk( NULL /* !! */, chunk_CONTAINER, id )
{
	XMP_Enforce( parent != NULL );

	this->containerType = containerType;
	this->newSize = 12;
	this->parent = parent;

	chunkVect* siblings = &parent->children;

	siblings->push_back( this );
}
