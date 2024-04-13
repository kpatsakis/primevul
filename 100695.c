void HTMLFormElement::finishRequestAutocomplete(AutocompleteResult result)
{
    RefPtr<Event> event;
    if (result == AutocompleteResultSuccess)
        event = Event::create(eventNames().autocompleteEvent);
    else if (result == AutocompleteResultErrorDisabled)
        event = AutocompleteErrorEvent::create("disabled");
    else if (result == AutocompleteResultErrorCancel)
        event = AutocompleteErrorEvent::create("cancel");
    else if (result == AutocompleteResultErrorInvalid)
        event = AutocompleteErrorEvent::create("invalid");

    event->setTarget(this);
    m_pendingAutocompleteEvents.append(event.release());

    if (!m_requestAutocompleteTimer.isActive())
        m_requestAutocompleteTimer.startOneShot(0);
}
