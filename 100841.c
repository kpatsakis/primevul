static ImageEventSender& errorEventSender()
{
    DEFINE_STATIC_LOCAL(ImageEventSender, sender, (eventNames().errorEvent));
    return sender;
}
