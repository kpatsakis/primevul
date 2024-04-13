void ValueChunk::write( RIFF_MetaHandler* handler, XMP_IO* file, bool isMainChunk )
{
	XIO::WriteUns32_LE( file, this->id );
	XIO::WriteUns32_LE( file, (XMP_Uns32)this->newSize - 8 );
	file->Write ( this->newValue.data() , (XMP_Int32)this->newSize - 8  );
}
