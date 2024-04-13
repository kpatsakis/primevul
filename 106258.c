void ContainerChunk::write( RIFF_MetaHandler* handler, XMP_IO* file, bool isMainChunk )
{
	if ( isMainChunk )
		file ->Rewind();

	XMP_Int64 chunkStart = file->Offset();
	XMP_Int64 chunkEnd = chunkStart + this->newSize;
	XMP_Enforce( chunkStart % 2 == 0 );
	chunkVect *rc = &this->children;

	XMP_Int64 childStart = chunkEnd;
	for ( XMP_Int32 chunkNo = (XMP_Int32)(rc->size() -1); chunkNo >= 0; chunkNo-- )
	{
		Chunk* cur = rc->at(chunkNo);

		if ( cur->newSize % 2 == 1 )
		{
			childStart--;
			file->Seek ( childStart, kXMP_SeekFromStart  );
			XIO::WriteUns8( file, 0 );
		}

		childStart-= cur->newSize;
		file->Seek ( childStart, kXMP_SeekFromStart  );
		switch ( cur->chunkType )
		{
			case chunk_GENERAL: //COULDDO enfore no change, since not write-out-able
				if ( cur->oldPos != childStart )
					XIO::Move( file, cur->oldPos, file, childStart, cur->oldSize );
				break;
			default:
				cur->write( handler, file, false );
				break;
		} // switch

	} // for
	XMP_Enforce ( chunkStart + 12 == childStart);
	file->Seek ( chunkStart, kXMP_SeekFromStart );

	XIO::WriteUns32_LE( file, this->id );
	XIO::WriteUns32_LE( file, (XMP_Uns32) this->newSize - 8 ); // validated in changesAndSize() above
	XIO::WriteUns32_LE( file, this->containerType );

}
