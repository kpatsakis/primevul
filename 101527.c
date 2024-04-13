pp::Buffer_Dev PDFiumEngine::GetFlattenedPrintData(const FPDF_DOCUMENT& doc) {
  pp::Buffer_Dev buffer;
  int page_count = FPDF_GetPageCount(doc);
  for (int i = 0; i < page_count; ++i) {
    FPDF_PAGE page = FPDF_LoadPage(doc, i);
    DCHECK(page);
    int flatten_ret = FPDFPage_Flatten(page, FLAT_PRINT);
    FPDF_ClosePage(page);
    if (flatten_ret == FLATTEN_FAIL)
      return buffer;
  }

  PDFiumMemBufferFileWrite output_file_write;
  if (FPDF_SaveAsCopy(doc, &output_file_write, 0)) {
    buffer = pp::Buffer_Dev(
        client_->GetPluginInstance(), output_file_write.size());
    if (!buffer.is_null()) {
      memcpy(buffer.data(), output_file_write.buffer().c_str(),
             output_file_write.size());
    }
  }
  return buffer;
}
