static uint32_t xhci_nec_challenge(uint32_t hi, uint32_t lo)
{
    uint32_t val;
    val = rotl(lo - 0x49434878, 32 - ((hi>>8) & 0x1F));
    val += rotl(lo + 0x49434878, hi & 0x1F);
    val -= rotl(hi ^ 0x49434878, (lo >> 16) & 0x1F);
    return ~val;
}
