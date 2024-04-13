void DTLS_RECORD_LAYER_set_saved_w_epoch(RECORD_LAYER *rl, unsigned short e)
{
    if (e == rl->d->w_epoch - 1) {
        memcpy(rl->d->curr_write_sequence,
               rl->write_sequence, sizeof(rl->write_sequence));
        memcpy(rl->write_sequence,
               rl->d->last_write_sequence, sizeof(rl->write_sequence));
    } else if (e == rl->d->w_epoch + 1) {
        memcpy(rl->d->last_write_sequence,
               rl->write_sequence, sizeof(unsigned char[8]));
        memcpy(rl->write_sequence,
               rl->d->curr_write_sequence, sizeof(rl->write_sequence));
    }
    rl->d->w_epoch = e;
}
