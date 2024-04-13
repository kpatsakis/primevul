void PostScript_MetaHandler::setTokenInfo(TokenFlag tFlag,XMP_Int64 offset,XMP_Int64 length)
{
	if (!(docInfoFlags&tFlag)&&tFlag>=kPS_ADOContainsXMP && tFlag<=kPS_EndPostScript)
	{
		size_t index=0;
		XMP_Uns64 flag=tFlag;
		while(flag>>=1) index++;
		fileTokenInfo[index].offsetStart=offset;
		fileTokenInfo[index].tokenlen=length;
		docInfoFlags|=tFlag;
	}
}
