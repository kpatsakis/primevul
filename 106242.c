ValueChunk::ValueChunk( ContainerChunk* parent, std::string value, XMP_Uns32 id ) : Chunk( parent, chunk_VALUE, id )
{
	this->oldValue = std::string();
	this->SetValue( value );
}
