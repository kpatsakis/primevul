static void findHotKeys(void) {
    redisReply *keys, *reply;
    unsigned long long counters[HOTKEYS_SAMPLE] = {0};
    sds hotkeys[HOTKEYS_SAMPLE] = {NULL};
    unsigned long long sampled = 0, total_keys, *freqs = NULL, it = 0;
    unsigned int arrsize = 0, i, k;
    double pct;

    /* Total keys pre scanning */
    total_keys = getDbSize();

    /* Status message */
    printf("\n# Scanning the entire keyspace to find hot keys as well as\n");
    printf("# average sizes per key type.  You can use -i 0.1 to sleep 0.1 sec\n");
    printf("# per 100 SCAN commands (not usually needed).\n\n");

    /* SCAN loop */
    do {
        /* Calculate approximate percentage completion */
        pct = 100 * (double)sampled/total_keys;

        /* Grab some keys and point to the keys array */
        reply = sendScan(&it);
        keys  = reply->element[1];

        /* Reallocate our freqs array if we need to */
        if(keys->elements > arrsize) {
            freqs = zrealloc(freqs, sizeof(unsigned long long)*keys->elements);

            if(!freqs) {
                fprintf(stderr, "Failed to allocate storage for keys!\n");
                exit(1);
            }

            arrsize = keys->elements;
        }

        getKeyFreqs(keys, freqs);

        /* Now update our stats */
        for(i=0;i<keys->elements;i++) {
            sampled++;
            /* Update overall progress */
            if(sampled % 1000000 == 0) {
                printf("[%05.2f%%] Sampled %llu keys so far\n", pct, sampled);
            }

            /* Use eviction pool here */
            k = 0;
            while (k < HOTKEYS_SAMPLE && freqs[i] > counters[k]) k++;
            if (k == 0) continue;
            k--;
            if (k == 0 || counters[k] == 0) {
                sdsfree(hotkeys[k]);
            } else {
                sdsfree(hotkeys[0]);
                memmove(counters,counters+1,sizeof(counters[0])*k);
                memmove(hotkeys,hotkeys+1,sizeof(hotkeys[0])*k);
            }
            counters[k] = freqs[i];
            hotkeys[k] = sdsnew(keys->element[i]->str);
            printf(
               "[%05.2f%%] Hot key '%s' found so far with counter %llu\n",
               pct, keys->element[i]->str, freqs[i]);
        }

        /* Sleep if we've been directed to do so */
        if(sampled && (sampled %100) == 0 && config.interval) {
            usleep(config.interval);
        }

        freeReplyObject(reply);
    } while(it != 0);

    if (freqs) zfree(freqs);

    /* We're done */
    printf("\n-------- summary -------\n\n");

    printf("Sampled %llu keys in the keyspace!\n", sampled);

    for (i=1; i<= HOTKEYS_SAMPLE; i++) {
        k = HOTKEYS_SAMPLE - i;
        if(counters[k]>0) {
            printf("hot key found with counter: %llu\tkeyname: %s\n", counters[k], hotkeys[k]);
            sdsfree(hotkeys[k]);
        }
    }

    exit(0);
}
