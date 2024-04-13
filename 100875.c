void HTMLInputElement::attach(const AttachContext& context)
{
    if (!m_hasType)
        updateType();

    HTMLTextFormControlElement::attach(context);

    m_inputTypeView->startResourceLoading();
    m_inputType->countUsage();

    if (document().focusedElement() == this)
        document().updateFocusAppearanceSoon(true /* restore selection */);
}
