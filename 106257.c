void Chunk::write( RIFF_MetaHandler* handler, XMP_IO* file , bool isMainChunk )
{
	throw new XMP_Error(kXMPErr_InternalFailure, "Chunk::write never to be called for unknown chunks.");
}
