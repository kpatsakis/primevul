XMPChunk::XMPChunk(Container* parent, WEBP_MetaHandler* handler)
  : Chunk(parent, handler)
{
    handler->packetInfo.offset = this->pos + 8;
    handler->packetInfo.length = (XMP_Int32) this->data.size();

    handler->xmpPacket.reserve(handler->packetInfo.length);
    handler->xmpPacket.assign(handler->packetInfo.length, '\0');
    handler->xmpPacket.insert(handler->xmpPacket.begin(), this->data.begin(),
                              this->data.end());

    handler->containsXMP = true; // last, after all possible failure

    handler->xmpChunk = this;
}
