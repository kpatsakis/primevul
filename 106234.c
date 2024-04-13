static void UTF8ToMacRoman ( const std::string & utf8, std::string * macRoman )
{
	macRoman->erase();
	bool inNonMRSpan = false;
	
	for ( const XMP_Uns8 * chPtr = (XMP_Uns8*)utf8.c_str(); *chPtr != 0; ++chPtr ) {	// ! Don't trust that char is unsigned.
		if ( *chPtr < 0x80 ) {
			(*macRoman) += (char)*chPtr;
			inNonMRSpan = false;
		} else {
			XMP_Uns32 cp = GetCodePoint ( &chPtr );
			--chPtr;	// Make room for the loop increment.
			XMP_Uns8  mr;
			for ( mr = 0; (mr < 0x80) && (cp != kMacRomanCP[mr]); ++mr ) {};	// Using std::map would be faster.
			if ( mr < 0x80 ) {
				(*macRoman) += (char)(mr+0x80);
				inNonMRSpan = false;
			} else if ( ! inNonMRSpan ) {
				(*macRoman) += '?';
				inNonMRSpan = true;
			}
		}
	}

}	// UTF8ToMacRoman
