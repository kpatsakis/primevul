void XMPChunk::write(WEBP_MetaHandler* handler)
{
    XMP_IO* file = handler->parent->ioRef;
    this->size = handler->xmpPacket.size();
    XIO::WriteUns32_LE(file, this->tag);
    XIO::WriteUns32_LE(file, (XMP_Uns32) this->size);
    file->Write(handler->xmpPacket.data(), (XMP_Int32) this->size);
    if (this->size & 1) {
        const XMP_Uns8 zero = 0;
        file->Write(&zero, 1);
    }
}
