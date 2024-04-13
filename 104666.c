pvscsi_request_cancelled(SCSIRequest *req)
{
    PVSCSIRequest *pvscsi_req = req->hba_private;
    PVSCSIState *s = pvscsi_req->dev;

    if (pvscsi_req->completed) {
        return;
    }

   if (pvscsi_req->dev->resetting) {
       pvscsi_req->cmp.hostStatus = BTSTAT_BUSRESET;
    } else {
       pvscsi_req->cmp.hostStatus = BTSTAT_ABORTQUEUE;
    }

    pvscsi_complete_request(s, pvscsi_req);
}
