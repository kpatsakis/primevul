bool PostScript_CheckFormat ( XMP_FileFormat format,
	                          XMP_StringPtr  filePath,
	                          XMP_IO*    fileRef,
	                          XMPFiles *     parent )
{
	IgnoreParam(filePath); IgnoreParam(parent);
	XMP_Assert ( (format == kXMP_EPSFile) || (format == kXMP_PostScriptFile) );

	return PostScript_Support::IsValidPSFile(fileRef,format) ;

}	// PostScript_CheckFormat
