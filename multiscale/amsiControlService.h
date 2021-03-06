#ifndef AMSI_CONTROLSERVICE_AMSI_H_
#define AMSI_CONTROLSERVICE_AMSI_H_
#include "amsiCoupling.h"
#include "amsiAssemblable.h"
#include "amsiCommunicationManager.h"
#include "amsiDataDistribution.h"
#include "amsiTaskManager.h"
#include "amsiUtilFunctions.h"
#include "amsiMPI.h"
#include <cstring>
#include <list>
#include <string>
namespace amsi
{
  /// this thing is a pile and all functionalities it provides (communication and migration, need to be factored out.
  class ControlService {
    public:
    static ControlService *Instance()
    {
      return instance = instance == NULL ? new ControlService() : instance;
    };
    void SetCommunicationManager(CommunicationManager *);
    void SetTaskManager(TaskManager *);
    TaskManager *GetTaskManager() { return task_man; }
    CommunicationManager *getCommunicationManager() { return comm_man; }
    void setScaleMain(const std::string &, ExecuteFunc);
    int Execute(int &argc, char **&argv, amsi::Multiscale &);
    size_t CommRelation_Define(const std::string &nm1, const std::string &nm2);
    size_t CreateCommPattern(const std::string &t1_dd, const std::string &t1,
                             const std::string &t2);
    void getPatternInfo(size_t rdd_id, int &, int &, CommPattern *&);
    // Remove and add data from comm pattern
    void RemoveData(size_t rdd_id, std::vector<int> &data);
    template <typename D,
              template <typename T, typename All = std::allocator<T> >
              class Container>
    size_t AddData(size_t rdd_id, Container<D> &objects, std::vector<int> &data,
                   int option = 0, void (*userFunc)() = NULL);
    // simpler addData that only effects the datadistribution and commpattern,
    // not the user data ordering always spreads the data evenly (make adddata
    // an object built ontop of the commpattern or datadist or something)
    size_t addData(size_t rdd_id, size_t &cnt);
    // todo move inxto commpattern
    bool amSender(size_t rdd_id);
    // todo move into commpatter
    bool amRecver(size_t rdd_id);
    // Migration functions
    void planMigration(std::vector<int> & migration_indices, size_t rdd_id, int option = 0, void (*userFunc)() = NULL);
    template <typename D, template<typename T, typename All = std::allocator<T> > class Container>
      void migration(std::vector< std::vector<char> > & migration_data, size_t rdd_id, Container<D> & objects);
    template <typename D, template <typename T, typename All = std::allocator<T> > class Container>
      void shareMigration(size_t rdd_id, Container<D> & objects);
    void CommPattern_Assemble(size_t rdd_id);
    size_t RecvCommPattern(const std::string & t1_dd_nm, const std::string & t1_nm,
                           const std::string & t2_nm, const std::string & t2_dd_nm);
    void CommPattern_Reconcile(size_t rdd_id);
    size_t CommPattern_UseInverted(size_t rdd_id, const std::string & dd,
                                   const std::string & t1, const std::string & t2);
    void CommPattern_UpdateInverted(size_t use_rdd_id,size_t create_rdd_id);
    void Assemble(size_t,Assemblable*);
    void Reconcile(size_t,Reconcilable*);
    template <typename D>
      void Assemble(size_t t_id, D & data, MPI_Datatype type);
    template <typename D>
      void Reconcile(size_t r_id, D & data, MPI_Datatype type);
    template <typename T>
      void scaleBroadcast(size_t r_id, T * buf);
    template <typename I, typename O>
      void aSendBroadcast(O out, size_t rid, I * bfr, size_t cnt);
    template <typename T>
      int aRecvBroadcastSize(size_t rid);
    template <typename I, typename O>
      void aRecvBroadcast(O out, size_t rid, I * bfr, size_t cnt);
    template <typename D, template <typename T,typename All = std::allocator<T> > class Container>
      void Communicate(size_t rdd_id,Container<D> & buffer,MPI_Datatype type);
    template <typename PRT_IO, typename DATA_IO>
      void Communicate(size_t rdd_id, PRT_IO cnts, DATA_IO bfr, MPI_Datatype tp);
  private:
    ControlService();
    ControlService(const ControlService &);
    ControlService & operator=(const ControlService &);
    static ControlService * instance;
    static size_t rel_dist_id_gen;
    CommunicationManager * comm_man;
    TaskManager * task_man;
    // map that stores the commpattern -> relation, data_distribution information
    typedef std::map<size_t, std::pair<size_t,size_t> > rddmap_t;
    rddmap_t rdd_map;
    // map that stores the commpattern -> recv_data_distribution information
    typedef std::map<size_t, size_t> rdd_dd_map_t;
    rdd_dd_map_t rdd_dd_map;
    typedef std::map<size_t,size_t> r_init_t;
    r_init_t r_init;
    // Temp location for migration data
    std::vector<int> m_send_to;
    std::vector<int> m_index;
    std::vector<int> m_t1process;
    std::vector<int> m_recv_from;
  };
} // namespace amsi
#include "amsiControlService_impl.h"
#endif
