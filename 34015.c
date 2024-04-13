c_pdf14trans_write_ctm(byte **ppbuf, const gs_pdf14trans_params_t *pparams)
{
    /* Note: We can't skip writing CTM if it is equal to pgs->ctm,
       because clist writer may skip this command for some bands.
       For a better result we need individual CTM for each band.
     */
    byte *pbuf = *ppbuf;
    int len, code;

    len = cmd_write_ctm_return_length_nodevice(&pparams->ctm);
    pbuf--; /* For cmd_write_ctm. */
    code = cmd_write_ctm(&pparams->ctm, pbuf, len);
    if (code < 0)
        return code;
    pbuf += len + 1;
    *ppbuf = pbuf;
    return 0;
}
