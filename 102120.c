String8 MediaHTTP::getMIMEType() const {
 if (mInitCheck != OK) {
 return String8("application/octet-stream");
 }

 String8 mimeType;
 status_t err = mHTTPConnection->getMIMEType(&mimeType);

 if (err != OK) {
 return String8("application/octet-stream");
 }

 return mimeType;
}
