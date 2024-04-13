void ValueChunk::SetValue( std::string value, bool optionalNUL /* = false */ )
{
	this->newValue.assign( value );
	if ( (! optionalNUL) || ((value.size() & 1) == 1) ) {
		this->newValue.append( 1, '\0' ); // append zero termination as explicit part of string
	}
	this->newSize = this->newValue.size() + 8;
}
