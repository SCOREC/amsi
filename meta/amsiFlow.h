#ifndef AMSI_FLOW_H_
#define AMSI_FLOW_H_
namespace amsi
{
  // fsm of scale status
  enum ScaleState
  {
    READY_IDLE,
    READY_ACTIVE,
    BLOCK_IDLE,
    BLOCK_ACTIVE,
    RUNNING,
    STT_CNT
  };
  enum ScaleTransition
  {
    ACTIVATE,
    IDLE,
    RELEASE,
    TRNS_CNT
  };
  int scale_status[STT_CNT][TRNS_CNT] = {
    {READY_ACTIVE,READY_IDLE,READY_IDLE},    // ready idle
    {RUNNING,READY_IDLE,READY_ACTIVE},       // ready active
    {BLOCK_ACTIVE,BLOCK_IDLE,READY_IDLE},    // block idle
    {BLOCK_ACTIVE,BLOCK_IDLE,READY_ACTIVE},  // block active
    {RUNNING,BLOCK_ACTIVE,READY_ACTIVE}      // running
  }
  ScaleState flowTransition(ScaleState stt, ScaleTransition trnstn)
  {
    return scale_status[stt][trsntn];
  }
}
#endif
