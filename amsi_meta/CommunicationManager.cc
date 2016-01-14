#include "CommunicationManager.h"
#include "CommPattern.h"
#include "CommPattern_Create.h"
#include "Task.h"
#include "TaskManager.h"
namespace amsi
{
    // assumes static initialization of communication relations
    int CommunicationManager::inter_comm_tag = 0;

    /// @brief Standard constructor.
    CommunicationManager::CommunicationManager() :
      relation_comms(),
      relations(),
      rtmap(),
      comm_patterns(),
      comm_pattern_build()
    {}

    /// @brief Defines a communication relationship between two tasks, should
    ///        only be called during initialization and adaptation phases of
    ///        AMSI operation.
    /// @param id1 Identifier for the sending task in the CommRelation
    /// @param is2 Identifier for the recving task in the CommRelation
    /// @return size_t Identifier for the CommRelation
    size_t CommunicationManager::defineRelation(size_t id1, size_t id2)
    {
      size_t result = 0;
      // check if relation already exists
      if(!CommRelation_Exists(id1,id2))
      {
	relations.insert(std::make_pair(id1,id2));
	result = CommRelation_GetID(id1,id2);
	rtmap[result] = std::make_pair(id1,id2);
      }

      return result;
    }

    /// @brief Retrieve a CommPattern by id
    /// @param rdd_id The id of the CommPattern
    /// @return CommPattern* Pointer to the identified CommPattern or NULL if
    ///                      no such pattern exists
    CommPattern * CommunicationManager::CommPattern_Get(size_t rdd_id)
    {
      CommPattern * result = NULL;

      if(comm_patterns.find(rdd_id) != comm_patterns.end())
	result = comm_patterns[rdd_id];

      return result;
    }
   
    /// @brief Determine if two tasks have a CommRelation defined between them
    /// @param id1 Identifier for the sending task in the CommRelation
    /// @param id2 Identifier for the recving task in the CommRelation
    /// @return bool Whether or not the described CommRelation exists
    bool CommunicationManager::CommRelation_Exists(size_t id1, size_t id2)
    {
      bool result = false;
      std::pair<relationmap_type::iterator,relationmap_type::iterator> ret;
      ret = relations.equal_range(id1);

      for(;ret.first != ret.second; ret.first++)
	if(ret.first->second == id2)
	  result = true;
      return result;
    }

    /// @brief EXPERIMENTAL: Initialize MPI intercommunicators for tasks which have a CommRelation defined
    /// @param tm A pointer to the current TaskManager
    void CommunicationManager::InitInterComms(TaskManager * tm)
    {
      for(relationmap_type::iterator it = relations.begin(), itend = relations.end(); it != itend; it++)
      {
	Task * tA = tm->Task_Get(it->first);
	Task * tB = tm->Task_Get(it->second);
	size_t r_id = CommRelation_GetID(it->first,it->second);

	int lrA = tA->localRank();
	int lrB = tB->localRank();
	
	Task * local_task = lrA >= 0 ? tA : lrB >= 0 ? tB : NULL;

	if(local_task)
	{
	  const MPI_Group grp_a = tA->group();
	  const MPI_Group grp_b = tB->group();
	  
	  // create an intercomm for the tasks if this rank is in one of them
	  MPI_Group inter_group;
	  MPI_Group_union(grp_a,grp_b,&inter_group);
	  MPI_Comm_create(MPI_COMM_WORLD,inter_group,&relation_comms[r_id]);
	}
      }
    }

    /// @brief Create a CommPattern using the specified information
    /// @param rdd_id The CommPattern identifier
    /// @param dd A pointer to the data distribution used for this CommPattern
    /// @param size_t1 The number of processes currently associated with the sending task
    /// @param size_t2 The number of processes currently associated with the recving task
    /// @param CommPatternAlgo A function pointer to the current/assigned CommPattern creation algorithm for this specific CommPattern
    void CommunicationManager::CommPattern_Create(size_t rdd_id, DataDistribution* dd, int size_t1, int size_t2, CommPatternAlgo func)
    {
      // look up the commpattern build function registered for use by this relation and dd 
      //std::cout << "Invoking the pattern creation algorithm" << std::endl;
      CommPattern * cp = (*func)((*dd),size_t1,size_t2);
      comm_patterns[rdd_id] = cp;

      //std::cout << (*cp);
    }

    /// @brief Create a placeholder CommPattern into which a pattern will be reconciled by a sending task
    /// @param rdd_id The CommPattern identifier
    /// @param t1s The number of processes currently associated with the sending task
    /// @param t2s The number of processes currently associated with the recving task
    void CommunicationManager::CommPattern_CreateRecv(size_t rdd_id,
						      int t1s,
						      int t2s)
    {
      comm_patterns[rdd_id] = new FullCommPattern(t1s,t2s);
    }

    /// @brief Invert an assembled CommPattern for reciprocal communication
    /// @param n_rdd_id The identifier for the new CommPattern created by the inversion process
    /// @param o_rdd_id The identifier for the old CommPattern to be inverted
    void CommunicationManager::CommPattern_Invert(size_t n_rdd_id,
						  size_t o_rdd_id)
    {
      comm_patterns[n_rdd_id] = CommPattern_CreateInverted(comm_patterns[o_rdd_id]);
    }

    /// @brief Retrieve the identifier for a specific CommRelation
    /// @param id1 The identifier for the sending task
    /// @param id2 The identifier for the recving task
    /// @return size_T The identifier the CommRelation specified, or 0 if such a relation does not exist
    size_t CommunicationManager::CommRelation_GetID(size_t id1, size_t id2)
    {
      // hacky, horrible, and not at all a correct method, placeholder till we put in something better (needs to be non-commutative as t1->t2 often has t2->t1)
//      return id1-id2;
      size_t result = 0;
      hash_combine(result,id1);
      hash_combine(result,id2);
      return result; // may be commutative.. which would be bad
    }

  MPI_Comm CommunicationManager::CommRelation_GetInterComm(size_t r_id)
  {
    MPI_Comm result = MPI_COMM_NULL;
    
    rtmap_t::iterator it = rtmap.find(r_id);
    if(it != rtmap.end())
      result = relation_comms[r_id];

    return result;
  }

    // should extract this from the class 
    /// @brief Get the identifier for a CommPattern (uniquely identified by a CommRelation and DataDistribution)
    /// @param r_id The identifier for the CommRelation on which the CommPattern is defined
    /// @param dd
    size_t CommunicationManager::RelationDistribution_GetID(size_t r_id, DataDistribution * dd)
    {
      size_t seed = 0;
      hash_combine(seed,r_id);
      hash_combine(seed,(void*)dd); // using the address of the DataDistribution should break this for parallel use... why hasn't it?

      return seed;
    }

    /// @brief Get the tasks associated with a specific CommRelation
    /// @param r_id The CommRelation identifier
    /// @return std::pair<size_t,size_t> A pair of task identifiers, the first is the sending task and the second is the recving task
    std::pair<size_t,size_t> CommunicationManager::Relation_GetTasks(size_t r_id)
    {
      std::pair<size_t,size_t> result = std::make_pair(0,0);
      rtmap_t::iterator it = rtmap.find(r_id);
      if(it != rtmap.end())
	result = rtmap[r_id];
      return result;
    }

} // namespace amsi
