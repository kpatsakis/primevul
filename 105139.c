Chunk::Chunk(Container* parent, XMP_Uns32 tag)
  : parent(parent), tag(tag)
{
    this->needsRewrite = true;
}
