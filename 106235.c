void TradQT_Manager::UpdateChangedBoxes ( MOOV_Manager * moovMgr )
{
	MOOV_Manager::BoxInfo udtaInfo;
	MOOV_Manager::BoxRef  udtaRef = moovMgr->GetBox ( "moov/udta", &udtaInfo );
	XMP_Assert ( (udtaRef != 0) || (udtaInfo.childCount == 0) );
	
	if ( udtaRef != 0 ) {	// Might not have been a moov/udta box in the parse.

	
		for ( XMP_Uns32 ordinal = udtaInfo.childCount; ordinal > 0; --ordinal ) {	// ! Go backwards because of deletions.
	
			MOOV_Manager::BoxInfo currInfo;
			MOOV_Manager::BoxRef  currRef = moovMgr->GetNthChild ( udtaRef, (ordinal-1), &currInfo );
			if ( currRef == 0 ) break;	// Sanity check, should not happen.
			if ( (currInfo.boxType >> 24) != 0xA9 ) continue;
			if ( currInfo.contentSize < 2+2+1 ) continue;	// These were skipped by ParseCachedBoxes.
			
			InfoMapPos infoPos = this->parsedBoxes.find ( currInfo.boxType );
			if ( infoPos == this->parsedBoxes.end() ) moovMgr->DeleteNthChild ( udtaRef, (ordinal-1) );
		
		}
	
	}
	
	
	InfoMapCPos infoPos = this->parsedBoxes.begin();
	InfoMapCPos infoEnd = this->parsedBoxes.end();
	
	for ( ; infoPos != infoEnd; ++infoPos ) {

		ParsedBoxInfo * qtItem = (ParsedBoxInfo*) &infoPos->second;
		if ( ! qtItem->changed ) continue;
		qtItem->changed = false;
		
		XMP_Uns32 qtTotalSize = 0;	// Total size of the QT values, ignoring empty values.
		for ( size_t i = 0, limit = qtItem->values.size(); i < limit; ++i ) {
			if ( ! qtItem->values[i].macValue.empty() ) {
				if ( qtItem->values[i].macValue.size() > 0xFFFF ) qtItem->values[i].macValue.erase ( 0xFFFF );
				qtTotalSize += (XMP_Uns32)(2+2 + qtItem->values[i].macValue.size());
			}
		}
		
		if ( udtaRef == 0 ) {	// Might not have been a moov/udta box in the parse.
			moovMgr->SetBox ( "moov/udta", 0, 0 );
			udtaRef = moovMgr->GetBox ( "moov/udta", &udtaInfo );
			XMP_Assert ( udtaRef != 0 );
		}
		
		if ( qtTotalSize == 0 ) {
		
			moovMgr->DeleteTypeChild ( udtaRef, qtItem->id );
		
		} else {
		
			
			RawDataBlock fullValue;
			fullValue.assign ( qtTotalSize, 0 );
			XMP_Uns8 * valuePtr = &fullValue[0];
		
			for ( size_t i = 0, limit = qtItem->values.size(); i < limit; ++i ) {
				XMP_Assert ( qtItem->values[i].macValue.size() <= 0xFFFF );
				XMP_Uns16 textSize = (XMP_Uns16)qtItem->values[i].macValue.size();
				if ( textSize == 0 ) continue;
				PutUns16BE ( textSize, valuePtr );									valuePtr += 2;
				PutUns16BE ( qtItem->values[i].macLang, valuePtr );					valuePtr += 2;
				memcpy ( valuePtr, qtItem->values[i].macValue.c_str(), textSize );	valuePtr += textSize;
			}
			

			MOOV_Manager::BoxInfo itemInfo;
			MOOV_Manager::BoxRef  itemRef = moovMgr->GetTypeChild ( udtaRef, qtItem->id, &itemInfo );
			
			if ( itemRef != 0 ) {
				moovMgr->SetBox ( itemRef, &fullValue[0], qtTotalSize );
			} else {
				moovMgr->AddChildBox ( udtaRef, qtItem->id, &fullValue[0], qtTotalSize );
			}
			
		}

	}

}	// TradQT_Manager::UpdateChangedBoxes
