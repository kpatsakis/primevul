TIFF_MetaHandler::TIFF_MetaHandler ( XMPFiles * _parent ) : psirMgr(0), iptcMgr(0)
{
	this->parent = _parent;
	this->handlerFlags = kTIFF_HandlerFlags;
	this->stdCharForm  = kXMP_Char8Bit;

}	// TIFF_MetaHandler::TIFF_MetaHandler
