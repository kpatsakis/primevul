static bool IsNameOfInlineEventHandler(const Vector<UChar, 32>& name) {
  const size_t kLengthOfShortestInlineEventHandlerName = 5;  // To wit: oncut.
  if (name.size() < kLengthOfShortestInlineEventHandlerName)
    return false;
  return name[0] == 'o' && name[1] == 'n';
}
