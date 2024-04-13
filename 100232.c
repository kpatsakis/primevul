void FrameLoader::load(const ResourceRequest& request, bool lockHistory)
{
    load(request, SubstituteData(), lockHistory);
}
