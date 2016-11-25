#ifndef AMSI_COMMPATTERN_CREATE_H_
#define AMSI_COMMPATTERN_CREATE_H_
namespace amsi
{
  class DataDistribution;
  class CommPattern;
  /// Current type definition alias of the function signature for all algorithms which calculate and create task-global communication patterns from an assembled data distribution defined on the local task-group
  typedef CommPattern*(*CommPatternAlgo)(DataDistribution&,int,int);
  /// Algorithm which creates a communication pattern in which each process in the recv-ing task will recv a roughly (+-1) even portion of the total number of pieces of data repressented by the DataDistribution
  CommPattern * CommPattern_CreateDistro(DataDistribution & dd, int s1, int s2);
} // namespace amsi

#endif
