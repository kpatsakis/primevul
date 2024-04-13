status_t MediaPlayerService::Client::getRetransmitEndpoint(
 struct sockaddr_in* endpoint)
{
 if (NULL == endpoint)
 return BAD_VALUE;

    sp<MediaPlayerBase> p = getPlayer();

 if (p != NULL)
 return p->getRetransmitEndpoint(endpoint);

 if (!mRetransmitEndpointValid)
 return NO_INIT;

 *endpoint = mRetransmitEndpoint;

 return NO_ERROR;
}
