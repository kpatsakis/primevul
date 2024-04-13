void ContainerChunk::release()
{
	Chunk* curChunk;
	while( ! this->children.empty() )
	{
		curChunk = this->children.back();
		delete curChunk;
		this->children.pop_back();
	}
}
