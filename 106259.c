void XMPChunk::write( RIFF_MetaHandler* handler, XMP_IO* file, bool isMainChunk )
{
	XIO::WriteUns32_LE( file, kChunk_XMP );
	XIO::WriteUns32_LE( file, (XMP_Uns32) this->newSize - 8 ); // validated in changesAndSize() above
	file->Write ( handler->xmpPacket.data(), (XMP_Int32)handler->xmpPacket.size()  );
}
