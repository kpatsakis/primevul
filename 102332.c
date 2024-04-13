static struct audio_usecase *get_usecase_from_id(struct audio_device *adev,
 audio_usecase_t uc_id)
{
 struct audio_usecase *usecase;
 struct listnode *node;

    list_for_each(node, &adev->usecase_list) {
        usecase = node_to_item(node, struct audio_usecase, adev_list_node);
 if (usecase->id == uc_id)
 return usecase;
 }
 return NULL;
}
