#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  string line;
  string name, memory, units;
  float memoryTotal, memoryFree;
  float memoryUtilization{0};
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> name >> memory >> units;  // MemTotal
    memoryTotal = std::stoi(memory);
    std::getline(filestream, line);
    linestream.clear();
    linestream.str(line);
    linestream >> name >> memory >> units;  // MemFree
    memoryFree = std::stoi(memory);

    memoryUtilization = (memoryTotal - memoryFree) / memoryTotal;
  }

  return memoryUtilization;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string key;
  string time {""};
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> time;
  }
  return std::stol(time);
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return  ActiveJiffies() + IdleJiffies();
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 

  // https://stackoverflow.com/questions/39066998/what-are-the-meaning-of-values-at-proc-pid-stat
  long int utime, stime, cutime, cstime;
  long activeJiffies {-1};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(filestream.is_open())
  {
    string line, useless_token;
    std::getline(filestream, line);
    std::istringstream linestream(line);

    for(int i = 0; i < 13; ++i)
    {
      linestream >> useless_token;
    }

    // 14 -> utime, 15 -> stime, 16 -> cutime, 17-> cstime
    linestream >> utime >> stime >> cutime >> cstime;

    activeJiffies = (utime + stime + cutime + cstime) / sysconf(_SC_CLK_TCK);
  }

  return activeJiffies; 
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto jiffies = CpuUtilization();

  return std::stol(jiffies[CPUStates::kUser_]) +
         std::stol(jiffies[CPUStates::kNice_]) +
         std::stol(jiffies[CPUStates::kSystem_]) +
         std::stol(jiffies[CPUStates::kIRQ_]) +
         std::stol(jiffies[CPUStates::kSoftIRQ_]) +
         std::stol(jiffies[CPUStates::kSteal_]);
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  auto jiffies = CpuUtilization();

  return std::stol(jiffies[CPUStates::kIdle_]) +
         std::stol(jiffies[CPUStates::kIOwait_]);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, cpu, value;
  vector<string> jiffies;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    linestream >> cpu;

    while (linestream >> value) {
      jiffies.push_back(value);
    }
  }

  return jiffies;
}

// helper function to find a value by its key in a file.
// the structure of the file must be: key value
string findValueInFileByKey(string path, string formatedKey) {
  string line;
  string key;
  string value;
  std::ifstream filestream(path);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == formatedKey) {
          return value;
        }
      }
    }
  }

  return "";
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string value =
      findValueInFileByKey(kProcDirectory + kStatFilename, "processes");
  if (!value.empty()) {
    return std::stoi(value);
  } else {
    return 0;
  }
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string value =
      findValueInFileByKey(kProcDirectory + kStatFilename, "procs_running");
  if (!value.empty()) {
    return std::stoi(value);
  } else {
    return 0;
  }
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string command {""};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(filestream.is_open())
  {
    string line;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> command;
  }

  return command; 
  
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string vmSize = findValueInFileByKey(kProcDirectory + std::to_string(pid) + kStatusFilename, "VmSize:");
  int ram = 0;
  if(!vmSize.empty())
  {
    ram = std::stof(vmSize)/ 1024;
  }


  return std::to_string(ram);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string uid = findValueInFileByKey(kProcDirectory + std::to_string(pid) + kStatusFilename, "Uid:");
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  //https://www.cyberciti.biz/faq/understanding-etcpasswd-file-format/
  string user_uid = Uid(pid);
  string name {"Unkonwn"};
  if(user_uid.empty())
  {
    return name;
  }

  std::ifstream filestream(kPasswordPath);
  
  if (filestream.is_open()) {
    string line, pass, uid;
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> pass >> uid) {
        if (uid == user_uid) {
          return name;
        }
      }
    }
  }

  return name;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  // The 22nd field in this file contains the process start time in jiffies (1/100th of a second).
  long int uptime {0};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(filestream.is_open())
  {
    string line;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    std::string field;
    for (int i = 1; i <= 22; ++i) {
      linestream >> field;
    }

    uptime = std::stol(field) / sysconf(_SC_CLK_TCK);

  }
  
  return uptime; 
  
}
