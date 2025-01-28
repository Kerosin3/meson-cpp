#include <strstream>

#include "async.hpp"

namespace async
{

// store Processors
std::map<handle_t, std::shared_ptr<Processor::DataProcessor>> g_ProcessorMapper;
handle_t
connect(std::size_t bulk_size)
{
  auto proc_ptr = std::make_shared<Processor::DataProcessor>(bulk_size);
  g_ProcessorMapper[proc_ptr.get()] =  proc_ptr;
  return proc_ptr.get();
}

void
receive(handle_t handle, const char* data, std::size_t size)
{
  std::string in = data;
  auto ptr = g_ProcessorMapper.find(handle);
  if (ptr != g_ProcessorMapper.end()) {
    ptr->second->receive_input(in);
  }
}

void
receiveCin(handle_t handle)
{
  auto ptr = g_ProcessorMapper.find(handle);
  if (ptr != g_ProcessorMapper.end()) {
    ptr->second->revc_cin();
  }
}
/*
void
disconnect(handle_t handle)
{
  auto ptr = ProcessorsMap.find(handle);
  if (ptr != ProcessorsMap.end()) {
    ProcessorsMap.erase(ptr);
  }
}
*/
void
func1()
{
  std::cout << "func 1\n";
}

void
func2()
{
  std::cout << "func 2\n";
}

}  // namespace async
