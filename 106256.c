std::string ContainerChunk::toString(XMP_Uns8 level )
{
	XMP_Int64 offset= 12; // compute offsets, just for informational purposes

	char buffer[256];
	snprintf( buffer, 255, "%.4s:%.4s, "
			"oldSize: 0x%8llX, "
			"newSize: 0x%.8llX, "
			"oldPos: 0x%.8llX\n",
			(char*)(&this->id), (char*)(&this->containerType), this->oldSize, this->newSize, this->oldPos );

	std::string r(buffer);
	chunkVectIter iter;
	for( iter = this->children.begin(); iter != this->children.end(); iter++ )
	{
		char buffer[256];
		snprintf( buffer, 250, "offset 0x%.8llX", offset );
		r += std::string ( level*4, ' ' ) + std::string( buffer ) + ":" + (*iter)->toString( level + 1 );
		offset += (*iter)->newSize;
		if ( offset % 2 == 1 )
			offset++;
	}
	return std::string(r);
}
