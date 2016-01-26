namespace amsi
{
  template <typename T>
    void pushTo(RankSet * rs, T begin)
  {
    rank_t sz  = rs->size();
    rank_t rnks[sz];
    rs->toArray(rnks);
    std::copy(rnks,rnks+sz,begin);
  }
  template <typename T>
    void insertFrom(RankSet * rs, T begin, T end)
  {
    for(T rnk = begin; rnk != end; rnk++)
      rs->insert(*rnk);
  }
}
