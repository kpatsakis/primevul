TIFF_MetaHandler::~TIFF_MetaHandler()
{

	if ( this->psirMgr != 0 ) delete ( this->psirMgr );
	if ( this->iptcMgr != 0 ) delete ( this->iptcMgr );

}	// TIFF_MetaHandler::~TIFF_MetaHandler
