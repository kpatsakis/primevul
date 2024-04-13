chunkVectIter ContainerChunk::getChild( Chunk* needle )
{
	chunkVectIter iter;
	for( iter = this->children.begin(); iter != this->children.end(); iter++ )
	{
		Chunk* temp1 = *iter;
		Chunk* temp2 = needle;
		if ( (*iter) == needle ) return iter;
	}
	return this->children.end();
}
