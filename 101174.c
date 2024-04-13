void Document::setDoctype(PassRefPtrWillBeRawPtr<DocumentType> docType)
{
    ASSERT(!m_docType || !docType);
    m_docType = docType;
    if (m_docType) {
        this->adoptIfNeeded(*m_docType);
        if (m_docType->publicId().startsWith("-//wapforum//dtd xhtml mobile 1.", TextCaseInsensitive))
            m_isMobileDocument = true;
    }
    styleEngine().clearResolver();
}
