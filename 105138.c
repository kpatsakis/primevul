Chunk::Chunk(Container* parent, WEBP_MetaHandler* handler)
{
    this->needsRewrite = (parent) ? parent->needsRewrite : false;
    this->parent = parent;

    XMP_IO* file = handler->parent->ioRef;
    this->pos = file->Offset();

    this->tag = XIO::ReadUns32_LE(file);

    this->size = XIO::ReadUns32_LE(file);

    if ((this->pos + this->size + 8) > handler->initialFileSize) {
        XMP_Throw("Bad RIFF chunk size", kXMPErr_BadFileFormat);
    }

    this->data.reserve((XMP_Int32) this->size);
    this->data.assign((XMP_Int32) this->size, '\0');
    file->ReadAll((void*)this->data.data(), (XMP_Int32) this->size);

    if (this->size & 1) {
        file->Seek(1, kXMP_SeekFromCurrent);
    }
}
