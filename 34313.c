ProcRenderDispatch (ClientPtr client)
{
    REQUEST(xReq);
    
    if (stuff->data < RenderNumberRequests)
	return (*ProcRenderVector[stuff->data]) (client);
    else
	return BadRequest;
}
