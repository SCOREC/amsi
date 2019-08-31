#ifndef AMSI_COMMUNICATION_MANAGER_H_
#define AMSI_COMMUNICATION_MANAGER_H_
#include "unordered_map.h"
#include "amsiCreateCoupling.h"
#include "amsiAddData.h"
#include "amsiPlanMigration.h"
#include <map>
#include <string>
#include <utility>
#include <vector>
namespace amsi
{
  class TaskManager;
  class CommPattern;
  enum Role
  {
    SENDER = 0,
    RECVER = 1
  };
  typedef std::map<size_t,CommPattern*> commpatternmap_type;
  class CommunicationManager
  {
  public:
    CommunicationManager();
    size_t defineRelation(size_t id1, size_t id2);
    size_t getRelationID(size_t id1, size_t id2);
    bool CommRelation_Exists(size_t id1, size_t id2);
    // will return null until the pattern for a relation is actually initialized
    CommPattern * getCommPattern(size_t rdd_id);
    std::pair<size_t,size_t> Relation_GetTasks(size_t r_id);
  protected:
    friend class ControlService;
    //size_t CommRelation_GetID(size_t id1, size_t id2);
    MPI_Comm CommRelation_GetInterComm(size_t r_id);
    size_t RelationDistribution_GetID(size_t r_id, DataDistribution * dd);
    /// called prior to execution of tasks
    void InitInterComms(TaskManager * tm);
    void CommPattern_Create(size_t rdd_id, DataDistribution * dd, int t1s, int t2s, CommPatternAlgo func);
    void CommPattern_CreateRecv(size_t rdd_id, int t1s, int t2s);
    void CommPattern_Invert(size_t n_rdd_id, size_t o_rdd_id);
  private:
    static int inter_comm_tag;
    typedef std::map<size_t,MPI_Comm> rcommsmap_type;
    rcommsmap_type relation_comms; // sender: comm
    typedef std::multimap<size_t,size_t> relationmap_type;
    relationmap_type relations;     // (sender_task, recver_task)
    typedef std::map<size_t, std::pair<size_t,size_t> > rtmap_t;
    rtmap_t rtmap;
    // actual communication patterns created during execution
    commpatternmap_type comm_patterns;
    std::map<size_t,CommPatternAlgo> comm_pattern_build;
  };
  size_t getRelationID(CommunicationManager * cm,
                       TaskManager * tm,
                       const std::string & t1_nm,
                       const std::string & t2_nm);
} // namespace amsi

#endif
