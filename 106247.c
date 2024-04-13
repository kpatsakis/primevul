void ContainerChunk::changesAndSize( RIFF_MetaHandler* handler )
{



	bool isRIFFContainer = (this->id == kChunk_RIFF);
	bool isLISTContainer = (this->id == kChunk_LIST);
	XMP_Enforce ( isRIFFContainer | isLISTContainer );

	XMP_Index childIndex;	// Could be local to the loops, this simplifies debuging. Need a signed type!
	Chunk * currChild;

	if ( this->children.empty() ) {
		if ( isRIFFContainer) {
			this->newSize = 12;	// Keep a minimal size container.
		} else {
			this->newSize = 0;	// Will get removed from parent in outer call.
		}
		this->hasChange = true;
		return;	// Nothing more to do without children.
	}


	for ( childIndex = (XMP_Index)this->children.size() - 1; childIndex > 0; --childIndex ) {

		currChild = this->children[childIndex];
		if ( currChild->chunkType != chunk_JUNK ) continue;

		if ( isRIFFContainer ) {
			Chunk * prevChild = this->children[childIndex-1];
			if ( prevChild->chunkType != chunk_JUNK ) continue;
			prevChild->oldSize += currChild->oldSize;
			prevChild->newSize += currChild->newSize;
			prevChild->hasChange = true;
		}

		this->children.erase ( this->children.begin() + childIndex );
		delete currChild;
		this->hasChange = true;

	}


	for ( childIndex = (XMP_Index)this->children.size() - 1; childIndex >= 0; --childIndex ) {

		currChild = this->children[childIndex];

		++handler->level;
		currChild->changesAndSize ( handler );
		--handler->level;

		if ( (currChild->newSize == 8) || (currChild->newSize == 0) ) {	// ! The newSIze is supposed to include the header.
			this->children.erase ( this->children.begin() + childIndex );
			delete currChild;
			this->hasChange = true;
		} else {
			this->hasChange |= currChild->hasChange;
			currChild->needSizeFix = (currChild->newSize != currChild->oldSize);
			if ( currChild->needSizeFix && (currChild->newSize > currChild->oldSize) &&
				 (this == handler->lastChunk) && (childIndex+1 == (XMP_Index)this->children.size()) ) {
				if ( currChild->oldSize != 0 ) currChild->needSizeFix = false;
			}
		}

	}


	if ( isRIFFContainer ) {

		for ( childIndex = 0; childIndex < (XMP_Index)this->children.size(); ++childIndex ) {

			currChild = this->children[childIndex];
			if ( ! currChild->needSizeFix ) continue;
			currChild->needSizeFix = false;

			XMP_Int64 sizeDiff = currChild->newSize - currChild->oldSize;	// Positive for growth.
			XMP_Uns8  padSize = (currChild->newSize & 1);	// Need a pad for odd size.


			Chunk * nextChild = 0;
			if ( childIndex+1 < (XMP_Index)this->children.size() ) nextChild = this->children[childIndex+1];

			if ( (nextChild != 0) && (nextChild->chunkType == chunk_JUNK) ) {
				if ( nextChild->newSize >= (9 + sizeDiff + padSize) ) {

					nextChild->newSize -= sizeDiff;
					nextChild->newSize -= padSize;
					nextChild->hasChange = true;
					continue;

				} else if (  nextChild->newSize == (sizeDiff + padSize)  ) {

					this->children.erase ( this->children.begin() + childIndex + 1 );
					delete nextChild;
					continue;

				}
			}


			if ( (sizeDiff + padSize) <= -9 ) {
				this->children.insert ( (this->children.begin() + childIndex + 1), new JunkChunk ( NULL, ((-sizeDiff) - padSize) ) );
				continue;
			}


			XMP_Index junkIndex;
			Chunk * junkChunk = 0;
			for ( junkIndex = 0; junkIndex < (XMP_Index)this->children.size(); ++junkIndex ) {
				junkChunk = this->children[junkIndex];
				if ( junkChunk->chunkType != chunk_JUNK ) continue;
				if ( (junkChunk->newSize >= (9 + currChild->newSize + padSize)) ||
					 (junkChunk->newSize == (currChild->newSize + padSize)) ) break;
			}

			if ( junkIndex < (XMP_Index)this->children.size() ) {


				if ( junkChunk->newSize == (currChild->newSize + padSize) ) {

					this->children[junkIndex] = currChild;
					delete junkChunk;

				} else {

					XMP_Assert ( junkChunk->newSize >= (9 + currChild->newSize + padSize) );
					junkChunk->newSize -= (currChild->newSize + padSize);
					junkChunk->hasChange = true;
					this->children.insert ( (this->children.begin() + junkIndex), currChild );
					if ( junkIndex < childIndex ) ++childIndex;	// The insertion moved the current child.

				}

				if ( currChild->oldSize != 0 ) {
					this->children[childIndex] = new JunkChunk ( 0, currChild->oldSize );	// Replace the old space with junk.
				} else {
					this->children.erase ( this->children.begin() + childIndex );	// Remove the newly created chunk's old location.
					--childIndex;	// Make the next loop iteration not skip a chunk.
				}

				continue;

			}


			bool isListInfo = (currChild->id == kChunk_LIST) && (currChild->chunkType == chunk_CONTAINER) &&
							  (((ContainerChunk*)currChild)->containerType == kType_INFO);

			if ( isListInfo && (handler->riffChunks.size() > 1) &&
				 (this->id == kChunk_RIFF) && (this != handler->lastChunk) ) {

				if ( currChild->oldSize != 0 ) {
					this->children[childIndex] = new JunkChunk ( 0, currChild->oldSize );
				} else {
					this->children.erase ( this->children.begin() + childIndex );
					--childIndex;	// Make the next loop iteration not skip a chunk.
				}

				delete currChild;
				continue;

			}


			if ( (this == handler->lastChunk) && (childIndex+1 == (XMP_Index)this->children.size()) ) continue;	// Already last.

			handler->lastChunk->children.push_back( currChild );
			if ( currChild->oldSize != 0 ) {
				this->children[childIndex] = new JunkChunk ( 0, currChild->oldSize );	// Replace the old space with junk.
			} else {
				this->children.erase ( this->children.begin() + childIndex );	// Remove the newly created chunk's old location.
				--childIndex;	// Make the next loop iteration not skip a chunk.
			}

		}

	}


	this->newSize = 12;	// Start with standard container header.
	for ( childIndex = 0; childIndex < (XMP_Index)this->children.size(); ++childIndex ) {
		currChild = this->children[childIndex];
		this->newSize += currChild->newSize;
		this->newSize += (this->newSize & 1);	// Round up if odd.
	}

	XMP_Validate ( (this->newSize <= 0xFFFFFFFFLL), "No single chunk may be above 4 GB", kXMPErr_Unimplemented );

}
