pvscsi_log2(uint32_t input)
{
    int log = 0;
    assert(input > 0);
    while (input >> ++log) {
    }
    return log;
}
