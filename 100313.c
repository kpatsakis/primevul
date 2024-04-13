Cache* cache()
{
    static Cache* staticCache = new Cache;
    return staticCache;
}
