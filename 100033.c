GF_SceneManager *gf_sm_new(GF_SceneGraph *graph)
{
	GF_SceneManager *tmp;

	if (!graph) return NULL;
	GF_SAFEALLOC(tmp, GF_SceneManager);
	if (!tmp) return NULL;
	tmp->streams = gf_list_new();
	tmp->scene_graph = graph;
	return tmp;
}
