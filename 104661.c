pvscsi_queue_pending_descriptor(PVSCSIState *s, SCSIDevice **d,
                                struct PVSCSIRingReqDesc *descr)
{
    PVSCSIRequest *pvscsi_req;
    uint8_t lun;

    pvscsi_req = g_malloc0(sizeof(*pvscsi_req));
    pvscsi_req->dev = s;
    pvscsi_req->req = *descr;
    pvscsi_req->cmp.context = pvscsi_req->req.context;
    QTAILQ_INSERT_TAIL(&s->pending_queue, pvscsi_req, next);

    *d = pvscsi_device_find(s, descr->bus, descr->target, descr->lun, &lun);
    if (*d) {
        pvscsi_req->lun = lun;
    }

    return pvscsi_req;
}
