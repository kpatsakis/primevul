bool XSSAuditor::filterBaseToken(const FilterTokenRequest& request)
{
    ASSERT(request.token.type() == HTMLToken::StartTag);
    ASSERT(hasName(request.token, baseTag));

    return eraseAttributeIfInjected(request, hrefAttr);
}
