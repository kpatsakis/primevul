void encode(ArgumentEncoder* encoder, CFBooleanRef boolean)
{
    encoder->encodeBool(CFBooleanGetValue(boolean));
}
