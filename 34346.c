SProcRenderDispatch (ClientPtr client)
{
    REQUEST(xReq);
    
    if (stuff->data < RenderNumberRequests)
	return (*SProcRenderVector[stuff->data]) (client);
    else
	return BadRequest;
}
