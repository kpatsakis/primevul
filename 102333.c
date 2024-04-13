static struct audio_usecase *get_usecase_from_type(struct audio_device *adev,
 usecase_type_t type)
{
 struct audio_usecase *usecase;
 struct listnode *node;

    list_for_each(node, &adev->usecase_list) {
        usecase = node_to_item(node, struct audio_usecase, adev_list_node);
 if (usecase->type & type)
 return usecase;
 }
 return NULL;
}
