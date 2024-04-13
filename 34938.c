jbig2_decode_mmr_consume(Jbig2MmrCtx *mmr, int n_bits)
{
    mmr->word <<= n_bits;
    mmr->bit_index += n_bits;
    while (mmr->bit_index >= 8) {
        mmr->bit_index -= 8;
        if (mmr->data_index + 4 < mmr->size)
            mmr->word |= (mmr->data[mmr->data_index + 4] << mmr->bit_index);
        mmr->data_index++;
    }
}
