void Chunk::changesAndSize( RIFF_MetaHandler* handler )
{
	hasChange = false; // unknown chunk ==> no change, naturally
	this->newSize = this->oldSize;
}
