void ewk_view_transition_to_commited_for_newpage(Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);

    ewk_view_disable_render(ewkView);
    priv->flags.viewCleared = false;
}
