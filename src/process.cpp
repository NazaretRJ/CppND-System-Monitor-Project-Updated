#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include <linux_parser.h>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid)
{
  command_ = LinuxParser::Command(pid_);
  user_ = LinuxParser::User(pid_);

}

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const{
  float cpu {0.0f}; 

  long int system_uptime = LinuxParser::UpTime();
  long int active_jiffies = LinuxParser::ActiveJiffies(pid_);
  long int process_uptime = LinuxParser::UpTime(pid_);

  if(system_uptime - process_uptime > 0)
  {
    cpu = active_jiffies / float(system_uptime - process_uptime);
  }

  return cpu;
  
}

// Return the command that generated this process
string Process::Command() { return command_; }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { 
  return LinuxParser::UpTime() - LinuxParser::UpTime(pid_); 
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return CpuUtilization() < a.CpuUtilization();
}