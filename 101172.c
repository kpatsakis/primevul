void Document::registerEventFactory(PassOwnPtr<EventFactoryBase> eventFactory)
{
    ASSERT(!eventFactories().contains(eventFactory.get()));
    eventFactories().add(eventFactory);
}
