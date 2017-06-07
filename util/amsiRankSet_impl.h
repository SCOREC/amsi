namespace amsi
{
  template <typename T>
    void pushTo(RankSet * rs, T begin)
  {
    int sz  = rs->size();
    int rnks[sz];
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
