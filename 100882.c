void HTMLInputElement::setValue(const String& value, ExceptionState& exceptionState, TextFieldEventBehavior eventBehavior)
{
    if (isFileUpload() && !value.isEmpty()) {
        exceptionState.throwDOMException(InvalidStateError, "This input element accepts a filename, which may only be programatically set to the empty string.");
        return;
    }
    setValue(value, eventBehavior);
}
