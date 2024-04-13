void HTMLInputElement::updateType()
{
    const AtomicString& newTypeName = InputType::normalizeTypeName(fastGetAttribute(typeAttr));
    bool hadType = m_hasType;
    m_hasType = true;
    if (m_inputType->formControlType() == newTypeName)
        return;

    if (hadType && !InputType::canChangeFromAnotherType(newTypeName)) {
        setAttribute(typeAttr, type());
        return;
    }

    RefPtr<InputType> newType = InputType::create(*this, newTypeName);
    removeFromRadioButtonGroup();

    bool didStoreValue = m_inputType->storesValueSeparateFromAttribute();
    bool didRespectHeightAndWidth = m_inputType->shouldRespectHeightAndWidthAttributes();

    m_inputTypeView->destroyShadowSubtree();
    lazyReattachIfAttached();

    m_inputType = newType.release();
    if (hasAuthorShadowRoot())
        m_inputTypeView = InputTypeView::create(*this);
    else
        m_inputTypeView = m_inputType;
    m_inputTypeView->createShadowSubtree();

    bool hasTouchEventHandler = m_inputTypeView->hasTouchEventHandler();
    if (hasTouchEventHandler != m_hasTouchEventHandler) {
        if (hasTouchEventHandler)
            document().didAddTouchEventHandler(this);
        else
            document().didRemoveTouchEventHandler(this);
        m_hasTouchEventHandler = hasTouchEventHandler;
    }

    setNeedsWillValidateCheck();

    bool willStoreValue = m_inputType->storesValueSeparateFromAttribute();

    if (didStoreValue && !willStoreValue && hasDirtyValue()) {
        setAttribute(valueAttr, AtomicString(m_valueIfDirty));
        m_valueIfDirty = String();
    }
    if (!didStoreValue && willStoreValue) {
        AtomicString valueString = fastGetAttribute(valueAttr);
        m_valueIfDirty = sanitizeValue(valueString);
    } else
        updateValueIfNeeded();

    setFormControlValueMatchesRenderer(false);
    m_inputTypeView->updateView();

    if (didRespectHeightAndWidth != m_inputType->shouldRespectHeightAndWidthAttributes()) {
        ASSERT(elementData());
        if (const Attribute* height = getAttributeItem(heightAttr))
            attributeChanged(heightAttr, height->value());
        if (const Attribute* width = getAttributeItem(widthAttr))
            attributeChanged(widthAttr, width->value());
        if (const Attribute* align = getAttributeItem(alignAttr))
            attributeChanged(alignAttr, align->value());
    }

    if (document().focusedElement() == this)
        document().updateFocusAppearanceSoon(true /* restore selection */);

    setChangedSinceLastFormControlChangeEvent(false);

    addToRadioButtonGroup();

    setNeedsValidityCheck();
    notifyFormStateChanged();
}
