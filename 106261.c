void JunkChunk::write( RIFF_MetaHandler* handler, XMP_IO* file, bool isMainChunk )
{
	XIO::WriteUns32_LE( file, kChunk_JUNK );		// write JUNK, never JUNQ
	XMP_Enforce( this->newSize < 0xFFFFFFFF );
	XMP_Enforce( this->newSize >= 8 );			// minimum size of any chunk
	XMP_Uns32 innerSize = (XMP_Uns32)this->newSize - 8;
	XIO::WriteUns32_LE( file, innerSize );

	while ( innerSize > kZeroBufferSize64K )
	{
		file->Write ( kZeroes64K , kZeroBufferSize64K  );
		innerSize -= kZeroBufferSize64K;
	}
	file->Write ( kZeroes64K , innerSize  );
}
