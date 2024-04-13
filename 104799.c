void mptsas_process_config(MPTSASState *s, MPIMsgConfig *req)
{
    PCIDevice *pci = PCI_DEVICE(s);

    MPIMsgConfigReply reply;
    const MPTSASConfigPage *page;
    size_t length;
    uint8_t type;
    uint8_t *data = NULL;
    uint32_t flags_and_length;
    uint32_t dmalen;
    uint64_t pa;

    mptsas_fix_config_endianness(req);

    QEMU_BUILD_BUG_ON(sizeof(s->doorbell_msg) < sizeof(*req));
    QEMU_BUILD_BUG_ON(sizeof(s->doorbell_reply) < sizeof(reply));

    /* Copy common bits from the request into the reply. */
    memset(&reply, 0, sizeof(reply));
    reply.Action      = req->Action;
    reply.Function    = req->Function;
    reply.MsgContext  = req->MsgContext;
    reply.MsgLength   = sizeof(reply) / 4;
    reply.PageType    = req->PageType;
    reply.PageNumber  = req->PageNumber;
    reply.PageLength  = req->PageLength;
    reply.PageVersion = req->PageVersion;

    type = req->PageType & MPI_CONFIG_PAGETYPE_MASK;
    if (type == MPI_CONFIG_PAGETYPE_EXTENDED) {
        type = req->ExtPageType;
        if (type <= MPI_CONFIG_PAGETYPE_MASK) {
            reply.IOCStatus = MPI_IOCSTATUS_CONFIG_INVALID_TYPE;
            goto out;
        }

        reply.ExtPageType = req->ExtPageType;
    }

    page = mptsas_find_config_page(type, req->PageNumber);

    switch(req->Action) {
    case MPI_CONFIG_ACTION_PAGE_DEFAULT:
    case MPI_CONFIG_ACTION_PAGE_HEADER:
    case MPI_CONFIG_ACTION_PAGE_READ_NVRAM:
    case MPI_CONFIG_ACTION_PAGE_READ_CURRENT:
    case MPI_CONFIG_ACTION_PAGE_READ_DEFAULT:
    case MPI_CONFIG_ACTION_PAGE_WRITE_CURRENT:
    case MPI_CONFIG_ACTION_PAGE_WRITE_NVRAM:
        break;

    default:
        reply.IOCStatus = MPI_IOCSTATUS_CONFIG_INVALID_ACTION;
        goto out;
    }

    if (!page) {
        page = mptsas_find_config_page(type, 1);
        if (page) {
            reply.IOCStatus = MPI_IOCSTATUS_CONFIG_INVALID_PAGE;
        } else {
            reply.IOCStatus = MPI_IOCSTATUS_CONFIG_INVALID_TYPE;
        }
        goto out;
    }

    if (req->Action == MPI_CONFIG_ACTION_PAGE_DEFAULT ||
        req->Action == MPI_CONFIG_ACTION_PAGE_HEADER) {
        length = page->mpt_config_build(s, NULL, req->PageAddress);
        if ((ssize_t)length < 0) {
            reply.IOCStatus = MPI_IOCSTATUS_CONFIG_INVALID_PAGE;
            goto out;
        } else {
            goto done;
        }
    }

    if (req->Action == MPI_CONFIG_ACTION_PAGE_WRITE_CURRENT ||
        req->Action == MPI_CONFIG_ACTION_PAGE_WRITE_NVRAM) {
        length = page->mpt_config_build(s, NULL, req->PageAddress);
        if ((ssize_t)length < 0) {
            reply.IOCStatus = MPI_IOCSTATUS_CONFIG_INVALID_PAGE;
        } else {
            reply.IOCStatus = MPI_IOCSTATUS_CONFIG_CANT_COMMIT;
        }
        goto out;
    }

    flags_and_length = req->PageBufferSGE.FlagsLength;
    dmalen = flags_and_length & MPI_SGE_LENGTH_MASK;
    if (dmalen == 0) {
        length = page->mpt_config_build(s, NULL, req->PageAddress);
        if ((ssize_t)length < 0) {
            reply.IOCStatus = MPI_IOCSTATUS_CONFIG_INVALID_PAGE;
            goto out;
        } else {
            goto done;
        }
    }

    if (flags_and_length & MPI_SGE_FLAGS_64_BIT_ADDRESSING) {
        pa = req->PageBufferSGE.u.Address64;
    } else {
        pa = req->PageBufferSGE.u.Address32;
    }

    /* Only read actions left.  */
    length = page->mpt_config_build(s, &data, req->PageAddress);
    if ((ssize_t)length < 0) {
        reply.IOCStatus = MPI_IOCSTATUS_CONFIG_INVALID_PAGE;
        goto out;
    } else {
        assert(data[2] == page->number);
        pci_dma_write(pci, pa, data, MIN(length, dmalen));
        goto done;
    }

    abort();

done:
    if (type > MPI_CONFIG_PAGETYPE_MASK) {
        reply.ExtPageLength = length / 4;
        reply.ExtPageType   = req->ExtPageType;
    } else {
        reply.PageLength    = length / 4;
    }

out:
    mptsas_fix_config_reply_endianness(&reply);
    mptsas_reply(s, (MPIDefaultReply *)&reply);
    g_free(data);
}
