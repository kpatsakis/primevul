void SystemClipboard::SetSelectionMode(bool selection_mode) {
  buffer_ = selection_mode ? mojom::ClipboardBuffer::kSelection
                           : mojom::ClipboardBuffer::kStandard;
}
