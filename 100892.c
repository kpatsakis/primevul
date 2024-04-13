FormControlState HTMLTextAreaElement::saveFormControlState() const
{
    return m_isDirty ? FormControlState(value()) : FormControlState();
}
