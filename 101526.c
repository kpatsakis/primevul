int PDFiumEngine::GetBlock(void* param, unsigned long position,
                           unsigned char* buffer, unsigned long size) {
  DocumentLoader* loader = static_cast<DocumentLoader*>(param);
  return loader->GetBlock(position, size, buffer);
}
