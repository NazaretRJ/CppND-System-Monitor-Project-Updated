#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();               // Return this process's ID
  std::string User();      // Return the user (name) that generated this process
  std::string Command();   // TODO: See src/process.cpp
  float CpuUtilization() const;  // Return this process's CPU utilization
  std::string Ram();       // Return this process's memory utilization
  long int UpTime();       // Return the age of this process (in seconds)
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

 private:
  int pid_{0};
  std::string user_{""};
  std::string ram_{"0%"};
  std::string command_{""};
};

#endif