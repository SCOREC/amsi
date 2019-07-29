#include "amsiMigration.h"
#include "PCU.h"
namespace amsi
{
#ifdef ZOLTAN
int getLocalCount(void * data, int *)
{
  return *static_cast<int*>(data);
}
void getWeights(void * data,
                int ,
                int ,
                ZOLTAN_ID_PTR global_ids,
                ZOLTAN_ID_PTR local_ids,
                int ,
                float * obj_wgts,
                int *)
{
  size_t int_sz= sizeof(int);
  char * d = static_cast<char*>(data);
  int offset = *static_cast<int*>((void*)d);
  int count = *static_cast<int*>((void*)(d+int_sz));
  double * weights = static_cast<double*>((void*)(d+2*int_sz));
  for(int ii = 0; ii < count; ii++)
  {
    local_ids[ii] = ii;
    global_ids[ii] = offset + ii;
    obj_wgts[ii] = weights[ii];
  }
}
void zoltanPlan(std::vector<int> & to_serialize,
                std::vector<int> & send_to,
                int global_offset,
                int local_count,
                double * weights,
                Zoltan_Struct * zs)
{
  size_t int_sz = sizeof(int);
  size_t dbl_ptr_sz = sizeof(double*);
  char * count_buffer = new char[int_sz];
  memcpy(count_buffer,&local_count,int_sz);
  Zoltan_Set_Fn(zs,
                ZOLTAN_NUM_OBJ_FN_TYPE,
                (void(*)()) &getLocalCount,
                count_buffer);
  char * weight_buffer = new char[2*int_sz+dbl_ptr_sz];
  memcpy(weight_buffer,&global_offset,int_sz);
  memcpy(weight_buffer+int_sz,&local_count,int_sz);
  memcpy(weight_buffer+2*int_sz,&weights,dbl_ptr_sz);
  Zoltan_Set_Fn(zs,
                ZOLTAN_OBJ_LIST_FN_TYPE,
                (void(*)()) &getWeights,
                weight_buffer);
  Zoltan_Set_Param(zs,"RETURN_LISTS","EXPORT");
  Zoltan_Set_Param(zs,"OBJ_WEIGHT_DIM","1");
  Zoltan_Set_Param(zs,"LB_METHOD","BLOCK");
  int do_lb = 0;
  int num_gid_entries = 0;
  int num_lid_entries = 0;
  int num_recv = 0;
  int num_send = 0;
  ZOLTAN_ID_TYPE * recv_gids; // global object IDs to recv
  ZOLTAN_ID_TYPE * recv_lids; // "1 part per process" so this is not used
  ZOLTAN_ID_TYPE * send_gids; // global object IDS to send
  ZOLTAN_ID_TYPE * send_lids; // "1 part per process" so this is not used
  int * recv_ranks; // ranks from which to recv each object
  int * send_ranks; // ranks to which to send objects
  Zoltan_LB_Balance(zs,
                    &do_lb,
                    &num_gid_entries,
                    &num_lid_entries,
                    &num_recv,
                    &recv_gids,
                    &recv_lids,
                    &recv_ranks,
                    &num_send,
                    &send_gids,
                    &send_lids,
                    &send_ranks);
  if(do_lb)
  {
    to_serialize.assign(send_lids,send_lids+num_send);
    send_to.assign(send_ranks,send_ranks+num_send);
  }
  Zoltan_LB_Free_Data(&recv_gids,
                      &recv_lids,
                      &recv_ranks,
                      &send_gids,
                      &send_lids,
                      &send_ranks);
  delete [] count_buffer;
  delete [] weight_buffer;
}
#endif
Migration::Migration(MPI_Comm c,
                     int a,
                     lb_fctn ua)
  : comm(c),
    comm_size(-1),
    algo(a),
    usr_algo(ua)
{
# ifdef ZOLTAN
  zs = Zoltan_Create(comm);
# endif
  MPI_Comm_size(comm,&comm_size);
}
void Migration::plan(std::vector<int> & to_serialize,
                     int local_size,
                     double * local_weights)
{
  bool dynamic = false;
  if(!local_weights)
  {
    dynamic = true;
    local_weights = new double[local_size]();
    for(int ii = 0; ii < local_size; ii++)
      local_weights[ii] = 1.0;
  }
  int offset = 0;
  switch(algo)
  {
  case USER_ALGO:
    (*usr_algo)(to_serialize,
                send_to,
                comm,
                local_size,
                local_weights);
    break;
  case ZOLTAN_ALGO:
#   ifdef ZOLTAN
    MPI_Scan(&local_size,&offset,1,MPI_INT,MPI_SUM,comm);
    offset -= local_size;
    zoltanPlan(to_serialize,
               send_to,
               offset,
               local_size,
               local_weights,
               zs);
#   else
    std::cerr << "Zoltan load balancing specified but AMSI is not configured with ZOLTAN!" << std::endl;
#   endif
    break;
  default:
    AMSI_DEBUG(std::cout << "Migration algorithm " << algo << " not implemented, no migration will be conducted." << std::endl);
  case NO_ALGO:
    break;
  }
  if(dynamic)
    delete [] local_weights;
}
void Migration::execute(std::vector< std::vector<char> > & data)
{
  PCU_Comm_Begin();
  int send_count = send_to.size();
  for(int current_send = 0; current_send < send_count; current_send++)
  {
    PCU_Comm_Write(send_to[current_send],
                   data[current_send].data(),
                   data[current_send].size());
  }
  PCU_Comm_Send();
  int recv_from = -1;
  void * recv = NULL;
  size_t recv_size = 0;
  while(PCU_Comm_Read(&recv_from,&recv,&recv_size))
  {
    data.push_back(std::vector<char>());
    data.back().resize(recv_size);
    memcpy(data.back().data(),recv,recv_size);
    recvd_from.push_back(recv_from);
  }
}
ScaleSensitiveMigration::ScaleSensitiveMigration(CommPattern * p,
                                                 MPI_Comm c,
                                                 int a,
                                                 lb_fctn ua)
  : Migration(c,a,ua),
    cp(p),
    send_indices()
{}
void ScaleSensitiveMigration::plan(std::vector<int> & to_serialize,
                                   int local_size,
                                   double * local_weights)
{
  Migration::plan(to_serialize,local_size,local_weights);
  send_indices = to_serialize;
}
void ScaleSensitiveMigration::execute(std::vector< std::vector<char> > & data)
{
  assert(send_to.size() == data.size());
  int rank = -1;
  MPI_Comm_rank(comm,&rank);
  size_t int_sz = sizeof(int);
  int send_count = send_to.size();
  for(int ii = 0; ii < send_count; ii++)
  {
    std::pair<int,int> coupled_rank_index = coupledInfoByIndex(cp,
                                                               RECVER,
                                                               rank,
                                                               send_indices[ii]);
    size_t data_sz = data[ii].size();
    data[ii].resize(data_sz+2*int_sz);
    char * d = data[ii].data();
    memcpy(d+data_sz,&coupled_rank_index.first,int_sz);
    memcpy(d+data_sz+int_sz,&coupled_rank_index.second,int_sz);
  }
  Migration::execute(data);
  std::vector<int> coupled_processes;
  std::vector<int> coupled_indices;
  // recv'd data
  for(unsigned ii = 0; ii < data.size(); ii++)
  {
    size_t data_sz = data[ii].size();
    char * d = data[ii].data();
    int coupled_process = *static_cast<int*>((void*)(d+data_sz-2*int_sz));
    int coupled_index = *static_cast<int*>((void*)(d+data_sz-int_sz));
    coupled_processes.push_back(coupled_process);
    coupled_indices.push_back(coupled_index);
  }
  if(data.size())
  {
  }
}
}//namespace
