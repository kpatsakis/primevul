ASCII85Encoder::~ASCII85Encoder() {
  if (str->isEncoder())
    delete str;
}
