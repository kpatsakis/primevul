void HTMLInputElement::stepDown(int n, ExceptionState& exceptionState)
{
    m_inputType->stepUp(-n, exceptionState);
}
