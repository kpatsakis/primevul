static uint8_t modrm_reg(uint8_t modrm)
{
    return (modrm >> 3) & 7;
}
