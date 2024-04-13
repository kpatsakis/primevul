void ValueChunk::changesAndSize( RIFF_MetaHandler* handler )
{
	if ( this->newValue.size() != this->oldValue.size() ) {
		this->hasChange = true;
	} else if ( strncmp ( this->oldValue.c_str(), this->newValue.c_str(), this->newValue.size() ) != 0 ) {
		this->hasChange = true;
	}
}
