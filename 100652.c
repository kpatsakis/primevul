bool RenderFrameImpl::ShouldUpdateSelectionTextFromContextMenuParams(
    const base::string16& selection_text,
    size_t selection_text_offset,
    const gfx::Range& selection_range,
    const ContextMenuParams& params) {
  base::string16 trimmed_selection_text;
  if (!selection_text.empty() && !selection_range.is_empty()) {
    const int start = selection_range.GetMin() - selection_text_offset;
    const size_t length = selection_range.length();
    if (start >= 0 && start + length <= selection_text.length()) {
      base::TrimWhitespace(selection_text.substr(start, length), base::TRIM_ALL,
                           &trimmed_selection_text);
    }
  }
  base::string16 trimmed_params_text;
  base::TrimWhitespace(params.selection_text, base::TRIM_ALL,
                       &trimmed_params_text);
  return trimmed_params_text != trimmed_selection_text;
}
