PassRefPtrWillBeRawPtr<HTMLCollection> Document::forms()
{
    return ensureCachedCollection<HTMLCollection>(DocForms);
}
