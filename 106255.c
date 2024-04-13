std::string Chunk::toString(XMP_Uns8 level )
{
	char buffer[256];
	snprintf( buffer, 255, "%.4s -- "
							"oldSize: 0x%.8llX,  "
							"newSize: 0x%.8llX,  "
							"oldPos: 0x%.8llX\n",
		(char*)(&this->id), this->oldSize, this->newSize, this->oldPos );
	return std::string(buffer);
}
