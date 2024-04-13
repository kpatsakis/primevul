Chunk::Chunk( ContainerChunk* parent, ChunkType c, XMP_Uns32 id )
{
	this->hasChange = false;
	this->chunkType = c; // base class assumption
	this->parent = parent;
	this->id = id;
	this->oldSize = 0;
	this->newSize = 8;
	this->oldPos = 0; // inevitable for ad-hoc
	this->needSizeFix = false;

	if ( this->parent != NULL )
	{
		this->parent->children.push_back( this );
		if( this->chunkType == chunk_VALUE )
			this->parent->childmap.insert( std::make_pair( this->id, (ValueChunk*) this ) );
	}
}
