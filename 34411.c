int __glXDisp_VendorPrivateWithReply(__GLXclientState *cl, GLbyte *pc)
{
    xGLXVendorPrivateReq *req = (xGLXVendorPrivateReq *) pc;
    GLint vendorcode = req->vendorCode;
    __GLXdispatchVendorPrivProcPtr proc;


    proc = (__GLXdispatchVendorPrivProcPtr)
      __glXGetProtocolDecodeFunction(& VendorPriv_dispatch_info,
				     vendorcode, 0);
    if (proc != NULL) {
	return (*proc)(cl, (GLbyte*)req);
    }

    cl->client->errorValue = vendorcode;
    return __glXError(GLXUnsupportedPrivateRequest);
}
