PostScript_MetaHandler::PostScript_MetaHandler ( XMPFiles * _parent ):dscFlags(0),docInfoFlags(0)
	,containsXMPHint(false),fileformat(kXMP_UnknownFile)
{
	this->parent = _parent;
	this->handlerFlags = kPostScript_HandlerFlags;
	this->stdCharForm = kXMP_Char8Bit;
	this->psHint = kPSHint_NoMarker;

}	// PostScript_MetaHandler::PostScript_MetaHandler
