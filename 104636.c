static QEMUSGList *pvscsi_get_sg_list(SCSIRequest *r)
{
    PVSCSIRequest *req = r->hba_private;

    trace_pvscsi_get_sg_list(req->sgl.nsg, req->sgl.size);

    return &req->sgl;
}
