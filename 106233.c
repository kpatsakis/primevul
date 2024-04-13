static void MacRomanToUTF8 ( const std::string & macRoman, std::string * utf8 )
{
	utf8->erase();
	
	for ( XMP_Uns8* chPtr = (XMP_Uns8*)macRoman.c_str(); *chPtr != 0; ++chPtr ) {	// ! Don't trust that char is unsigned.
		if ( *chPtr < 0x80 ) {
			(*utf8) += (char)*chPtr;
		} else {
			(*utf8) += kMacRomanUTF8[(*chPtr)-0x80];
		}
	}

}	// MacRomanToUTF8
