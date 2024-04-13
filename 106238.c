ContainerChunk::ContainerChunk( ContainerChunk* parent, RIFF_MetaHandler* handler ) : Chunk( parent, handler, false, chunk_CONTAINER )
{
	bool repairMode = ( 0 != ( handler->parent->openFlags & kXMPFiles_OpenRepairFile ));

	try
	{
		XMP_IO* file = handler->parent->ioRef;
		XMP_Uns8 level = handler->level;

		this->containerType = XIO::ReadUns32_LE( file );

		if ( level == 0 && handler->riffChunks.size() > 0 )
		{
			XMP_Validate( handler->parent->format == kXMP_AVIFile, "only AVI may have multiple top-level chunks", kXMPErr_BadFileFormat );
			XMP_Validate( this->containerType == kType_AVIX, "all chunks beyond main chunk must be type AVIX", kXMPErr_BadFileFormat );
		}

		bool hasSubChunks = ( ( this->id == kChunk_RIFF ) ||
							  ( this->id == kChunk_LIST && this->containerType == kType_INFO ) ||
							  ( this->id == kChunk_LIST && this->containerType == kType_Tdat )
						  );
		XMP_Int64 endOfChunk = this->oldPos + this->oldSize;

		if ( (level == 0) && repairMode && (endOfChunk > handler->oldFileSize) )
		{
			endOfChunk = handler->oldFileSize; // assign actual file size
			this->oldSize = endOfChunk - this->oldPos; //reversely calculate correct oldSize
		}

		XMP_Validate( endOfChunk <= handler->oldFileSize, "offset beyond EoF", kXMPErr_BadFileFormat );

		Chunk* curChild = 0;
		if ( hasSubChunks )
		{
			handler->level++;
			while ( file->Offset() < endOfChunk )
			{
				curChild = RIFF::getChunk( this, handler );

				if ( file->Offset() % 2 == 1 )
				{
					XMP_Uns8 pad;
					file->Read ( &pad, 1 );  // Read the pad, tolerate being at EOF.

				}

				if ( (containerType== kType_INFO || containerType == kType_Tdat)
						&& ( curChild->chunkType == chunk_JUNK ) )
				{
						this->children.pop_back();
						delete curChild;
				} // for other chunks: join neighouring Junk chunks into one
				else if ( (curChild->chunkType == chunk_JUNK) && ( this->children.size() >= 2 ) )
				{
					Chunk* prevChunk = this->children.at( this->children.size() - 2 );
					if ( prevChunk->chunkType == chunk_JUNK )
					{
						prevChunk->oldSize += curChild->oldSize;
						prevChunk->newSize += curChild->newSize;
						XMP_Enforce( prevChunk->oldSize == prevChunk->newSize );
						this->children.pop_back();
						delete curChild;
					}
				}
			}
			handler->level--;
			XMP_Validate( file->Offset() == endOfChunk, "subchunks exceed outer chunk size", kXMPErr_BadFileFormat );

			if ( level==1 && this->id==kChunk_LIST && this->containerType == kType_INFO )
				handler->listInfoChunk = this;
			if ( level==1 && this->id==kChunk_LIST && this->containerType == kType_Tdat )
				handler->listTdatChunk = this;
		}
		else // skip non-interest container chunk
		{
			file->Seek ( (this->oldSize - 8 - 4), kXMP_SeekFromCurrent );
		} // if - else

	} // try
	catch (XMP_Error& e) {
		this->release(); // free resources
		if ( this->parent != 0)
			this->parent->children.pop_back(); // hereby taken care of, so removing myself...

		throw e;         // re-throw
	}
}
