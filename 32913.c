  psh_hint_overlap( PSH_Hint  hint1,
                    PSH_Hint  hint2 )
  {
    return hint1->org_pos + hint1->org_len >= hint2->org_pos &&
           hint2->org_pos + hint2->org_len >= hint1->org_pos;
  }
