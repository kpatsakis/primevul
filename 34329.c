RenderClientCallback (CallbackListPtr	*list,
		      pointer		closure,
		      pointer		data)
{
    NewClientInfoRec	*clientinfo = (NewClientInfoRec *) data;
    ClientPtr		pClient = clientinfo->client;
    RenderClientPtr	pRenderClient = GetRenderClient (pClient);

    pRenderClient->major_version = 0;
    pRenderClient->minor_version = 0;
}
