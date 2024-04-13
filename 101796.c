KURL ImageLoader::ImageSourceToKURL(AtomicString image_source_url) const {
  KURL url;

  Document& document = element_->GetDocument();
  if (!document.IsActive())
    return url;

  if (!image_source_url.IsNull()) {
    String stripped_image_source_url =
        StripLeadingAndTrailingHTMLSpaces(image_source_url);
    if (!stripped_image_source_url.IsEmpty())
      url = document.CompleteURL(stripped_image_source_url);
  }
  return url;
}
