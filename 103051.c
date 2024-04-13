static void lsi_request_cancelled(SCSIRequest *req)
{
    LSIState *s = LSI53C895A(req->bus->qbus.parent);
    lsi_request *p = req->hba_private;

    req->hba_private = NULL;
    lsi_request_free(s, p);
    scsi_req_unref(req);
}
