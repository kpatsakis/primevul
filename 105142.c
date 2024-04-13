void Chunk::write(WEBP_MetaHandler* handler)
{
    XMP_IO* file = handler->parent->ioRef;
    if (this->needsRewrite) {
        this->pos = file->Offset();
        XIO::WriteUns32_LE(file, this->tag);
        XIO::WriteUns32_LE(file, (XMP_Uns32) this->size);
        file->Write(this->data.data(), (XMP_Int32) this->size);
    }
    else {
        file->Seek(this->pos + this->size + 8, kXMP_SeekFromStart);
    }
    if (this->size & 1) {
        const XMP_Uns8 zero = 0;
        file->Write(&zero, 1);
    }
}
