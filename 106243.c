ValueChunk::ValueChunk( ContainerChunk* parent, RIFF_MetaHandler* handler ) : Chunk( parent, handler, false, chunk_VALUE )
{
	XMP_IO* file = handler->parent->ioRef;
	XMP_Uns8 level = handler->level;


	XMP_Int32 length = (XMP_Int32) this->oldSize - 8;
	this->oldValue.reserve( length );
	this->oldValue.assign( length + 1, '\0' );
	file->ReadAll ( (void*)this->oldValue.data(), length );

	this->newValue = this->oldValue;
	this->newSize = this->oldSize;
}
