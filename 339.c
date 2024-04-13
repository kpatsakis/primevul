void encode(ArgumentEncoder* encoder, CFDateRef date)
{
    encoder->encodeDouble(CFDateGetAbsoluteTime(date));
}
