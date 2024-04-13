PostScript_MetaHandler::TokenLocation& PostScript_MetaHandler::getTokenInfo(TokenFlag tFlag)
{
	if ((docInfoFlags&tFlag)&&tFlag>=kPS_ADOContainsXMP && tFlag<=kPS_EndPostScript)
	{
		size_t index=0;
		XMP_Uns64 flag=tFlag;
		while(flag>>=1) index++;
		return fileTokenInfo[index];
	}
	return fileTokenInfo[kPS_NoData];
}
