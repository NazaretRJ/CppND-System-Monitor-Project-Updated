#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    processes_.clear();    
    auto pids = LinuxParser::Pids();

    //ncurses display will only show 10 process
    // So this can be limited
    for(unsigned int i = 0; i < pids.size() && i < 10; ++i)
    {
        Process proc(pids[i]);
        //you do not need to call the constructor when inserting an element using emplace_back into the vector of some class 
        //if the constructor of the class has an appropriate definition that can be called on that set of arguments!
        //So this line can be replaced with: processes_.emplace_back(pid);
        processes_.push_back(proc);
    }

    sort(processes_.rbegin(), processes_.rend());
    
    return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }