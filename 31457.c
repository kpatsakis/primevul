int EC_GROUP_get_trinomial_basis(const EC_GROUP *group, unsigned int *k)
{
    if (group == NULL)
        return 0;

    if (EC_GROUP_method_of(group)->group_set_curve !=
        ec_GF2m_simple_group_set_curve || !((group->poly[0] != 0)
                                            && (group->poly[1] != 0)
                                            && (group->poly[2] == 0))) {
        ECerr(EC_F_EC_GROUP_GET_TRINOMIAL_BASIS,
              ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED);
        return 0;
    }

    if (k)
        *k = group->poly[1];

    return 1;
}
