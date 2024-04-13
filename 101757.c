bool XSSAuditor::FilterButtonToken(const FilterTokenRequest& request) {
  DCHECK_EQ(request.token.GetType(), HTMLToken::kStartTag);
  DCHECK(HasName(request.token, buttonTag));

  return EraseAttributeIfInjected(request, formactionAttr, kURLWithUniqueOrigin,
                                  kSrcLikeAttributeTruncation);
}
