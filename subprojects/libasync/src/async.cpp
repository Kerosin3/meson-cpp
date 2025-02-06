#include "async.hpp"

namespace async
{

// store Processors
std::map<handle_t, std::shared_ptr<Proc::ProcessorHub>> g_ProcessorMapper;
handle_t
connect(std::size_t bulk_size)
{
  auto proc_ptr = std::make_shared<Proc::ProcessorHub>(bulk_size);
  g_ProcessorMapper[proc_ptr.get()] =  proc_ptr;
  return proc_ptr.get();
}

void
receive(handle_t handle, const char* data, std::size_t size)
{
  std::string in_str = data;
  auto ptr = g_ProcessorMapper.find(handle);
  if (ptr != g_ProcessorMapper.end()) {
    ptr->second->receive_input(in_str);
  }
}

void
disconnect(handle_t handle)
{
  auto ptr = g_ProcessorMapper.find(handle);
  if (ptr != g_ProcessorMapper.end()) {
    ptr->second->finish();
  }
}
}  // namespace async
