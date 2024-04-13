static int v9fs_do_readdir(V9fsPDU *pdu,
                           V9fsFidState *fidp, int32_t max_count)
{
    size_t size;
    V9fsQID qid;
    V9fsString name;
    int len, err = 0;
    int32_t count = 0;
    off_t saved_dir_pos;
    struct dirent *dent;

    /* save the directory position */
    saved_dir_pos = v9fs_co_telldir(pdu, fidp);
    if (saved_dir_pos < 0) {
        return saved_dir_pos;
    }

    while (1) {
        v9fs_readdir_lock(&fidp->fs.dir);

        err = v9fs_co_readdir(pdu, fidp, &dent);
        if (err || !dent) {
            break;
        }
        v9fs_string_init(&name);
        v9fs_string_sprintf(&name, "%s", dent->d_name);
        if ((count + v9fs_readdir_data_size(&name)) > max_count) {
            v9fs_readdir_unlock(&fidp->fs.dir);

            /* Ran out of buffer. Set dir back to old position and return */
            v9fs_co_seekdir(pdu, fidp, saved_dir_pos);
            v9fs_string_free(&name);
            return count;
        }
        /*
         * Fill up just the path field of qid because the client uses
         * only that. To fill the entire qid structure we will have
         * to stat each dirent found, which is expensive
         */
        size = MIN(sizeof(dent->d_ino), sizeof(qid.path));
        memcpy(&qid.path, &dent->d_ino, size);
        /* Fill the other fields with dummy values */
        qid.type = 0;
        qid.version = 0;

        /* 11 = 7 + 4 (7 = start offset, 4 = space for storing count) */
        len = pdu_marshal(pdu, 11 + count, "Qqbs",
                          &qid, dent->d_off,
                          dent->d_type, &name);

        v9fs_readdir_unlock(&fidp->fs.dir);

        if (len < 0) {
            v9fs_co_seekdir(pdu, fidp, saved_dir_pos);
            v9fs_string_free(&name);
            return len;
        }
        count += len;
        v9fs_string_free(&name);
        saved_dir_pos = dent->d_off;
    }

    v9fs_readdir_unlock(&fidp->fs.dir);

    if (err < 0) {
        return err;
    }
    return count;
}
