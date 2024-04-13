bool PostScript_MetaHandler::ExtractDocInfoDict(IOBuffer &ioBuf)
{
	XMP_Uns8 ch;
	XMP_IO* fileRef = this->parent->ioRef;
	XMP_Int64 endofDocInfo=(ioBuf.ptr-ioBuf.data)+ioBuf.filePos;
	if ( ! CheckFileSpace ( fileRef, &ioBuf, 1 ) ) return false;
	if ( IsWhitespace (*ioBuf.ptr))
	{
		if ( ! ( PostScript_Support::SkipTabsAndSpaces(fileRef, ioBuf))) return false;
		if ( ! CheckFileSpace ( fileRef, &ioBuf, kPSContainsPdfmarkString.length() ) ) return false;
		if ( ! CheckBytes ( ioBuf.ptr,  Uns8Ptr(kPSContainsPdfmarkString.c_str()), kPSContainsPdfmarkString.length() ) ) return false;
		while(true)
		{
			if ( ! PostScript_Support::RevCheckFileSpace ( fileRef, &ioBuf, 1 ) ) return false;
			ch=*ioBuf.ptr;
			--ioBuf.ptr;
			if (ch=='/') break;//slash of /DOCINFO
		} 
		while(true)
		{
			if ( ! PostScript_Support::RevCheckFileSpace ( fileRef, &ioBuf, 1 ) ) return false;
			if (!IsWhitespace(*ioBuf.ptr)) break;
			--ioBuf.ptr;
		} 
						
		bool findingkey=false;
		std::string key, value;
		while(true)
		{
			XMP_Uns32 noOfMarks=0;
			if ( ! PostScript_Support::RevCheckFileSpace ( fileRef, &ioBuf, 1 ) ) return false;
			if (*ioBuf.ptr==')')
			{
				--ioBuf.ptr;
				while(true)
				{
					if (*ioBuf.ptr=='(')
					{
						if(findingkey)
						{
							reverse(key.begin(), key.end());
							reverse(value.begin(), value.end());
							RegisterKeyValue(key,value);
						}
						if ( ! PostScript_Support::RevCheckFileSpace ( fileRef, &ioBuf, 1 ) ) return false;
						--ioBuf.ptr;
						findingkey=!findingkey;
						break;
					}
					else
					{
						if ( ! PostScript_Support::RevCheckFileSpace ( fileRef, &ioBuf, 1 ) ) return false;
						if (findingkey)
							key+=*ioBuf.ptr;
						else
							value+=*ioBuf.ptr;
						--ioBuf.ptr;
					}
				}
			}
			else if(*ioBuf.ptr=='[')
			{
				break;
			}
			else
			{
				while(true)
				{
					if ( ! PostScript_Support::RevCheckFileSpace ( fileRef, &ioBuf, 1 ) ) return false;
					if (findingkey)
						key+=*ioBuf.ptr;
					else
						value+=*ioBuf.ptr;
					--ioBuf.ptr;
					if (*ioBuf.ptr=='/')
					{
						if(findingkey)
						{
							reverse(key.begin(), key.end());
							reverse(value.begin(), value.end());
							RegisterKeyValue(key,value);
						}
						if ( ! PostScript_Support::RevCheckFileSpace ( fileRef, &ioBuf, 1 ) ) return false;
						--ioBuf.ptr;
						findingkey=!findingkey;
						break;
					}
					else if(IsWhitespace(*ioBuf.ptr))
					{
						break;
					}
				}
			}
			while(true)
			{
				if ( ! PostScript_Support::RevCheckFileSpace ( fileRef, &ioBuf, 1 ) ) return false;
				if (!IsWhitespace(*ioBuf.ptr)) break;
				--ioBuf.ptr;
			} 
						
		}
						
		fileRef->Rewind();	
		FillBuffer (fileRef, endofDocInfo, &ioBuf );
		return true;
	}//white space not after DOCINFO
	return false;
}
