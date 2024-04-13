void SystemClipboard::WriteHTML(const String& markup,
                                const KURL& document_url,
                                const String& plain_text,
                                SmartReplaceOption smart_replace_option) {
  String text = plain_text;
#if defined(OS_WIN)
  ReplaceNewlinesWithWindowsStyleNewlines(text);
#endif
  ReplaceNBSPWithSpace(text);

  clipboard_->WriteHtml(mojom::ClipboardBuffer::kStandard,
                        NonNullString(markup), document_url);
  clipboard_->WriteText(mojom::ClipboardBuffer::kStandard, NonNullString(text));
  if (smart_replace_option == kCanSmartReplace)
    clipboard_->WriteSmartPasteMarker(mojom::ClipboardBuffer::kStandard);
  clipboard_->CommitWrite(mojom::ClipboardBuffer::kStandard);
}
