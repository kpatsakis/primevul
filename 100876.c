CheckedRadioButtons* HTMLInputElement::checkedRadioButtons() const
{
    if (!isRadioButton())
        return 0;
    if (HTMLFormElement* formElement = form())
        return &formElement->checkedRadioButtons();
    if (inDocument())
        return &document().formController()->checkedRadioButtons();
    return 0;
}
