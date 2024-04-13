int __glXDisp_VendorPrivate(__GLXclientState *cl, GLbyte *pc)
{
    xGLXVendorPrivateReq *req = (xGLXVendorPrivateReq *) pc;
    GLint vendorcode = req->vendorCode;
    __GLXdispatchVendorPrivProcPtr proc;


    proc = (__GLXdispatchVendorPrivProcPtr)
      __glXGetProtocolDecodeFunction(& VendorPriv_dispatch_info,
				     vendorcode, 0);
    if (proc != NULL) {
	(*proc)(cl, (GLbyte*)req);
	return Success;
    }

    cl->client->errorValue = req->vendorCode;
    return __glXError(GLXUnsupportedPrivateRequest);
}
