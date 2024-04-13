bool TIFF_CheckFormat ( XMP_FileFormat format,
	                    XMP_StringPtr  filePath,
                        XMP_IO*    fileRef,
                        XMPFiles *     parent )
{
	IgnoreParam(format); IgnoreParam(filePath); IgnoreParam(parent);
	XMP_Assert ( format == kXMP_TIFFFile );

	enum { kMinimalTIFFSize = 4+4+2+12+4 };	// Header plus IFD with 1 entry.

	fileRef->Rewind ( );
	if ( ! XIO::CheckFileSpace ( fileRef, kMinimalTIFFSize ) ) return false;

	XMP_Uns8 buffer [4];
	fileRef->Read ( buffer, 4 );
	
	bool leTIFF = CheckBytes ( buffer, "\x49\x49\x2A\x00", 4 );
	bool beTIFF = CheckBytes ( buffer, "\x4D\x4D\x00\x2A", 4 );

	return (leTIFF | beTIFF);

}	// TIFF_CheckFormat
