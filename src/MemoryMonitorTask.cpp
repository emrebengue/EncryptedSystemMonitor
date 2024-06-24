#include "MemoryMonitorTask.h"
#include <Poco/TaskManager.h>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <linux/sysinfo.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <thread>

MemoryMonitorTask::MemoryMonitorTask(const std::string& name) : Poco::Task(name), log_file("memory_monitor.log") {
    std::cout << "-------------Initializing MemoryMonitorTask-------------\n" << std::endl;
    logFile.open(log_file, std::ios::out | std::ios::app);

    if(logFile.is_open()){
        logFile << log_file << " Log file for Memory monitoring has been initialized\n" << std::endl;
    }

    
}   

MemoryMonitorTask::~MemoryMonitorTask(){
    if(logFile.is_open()){
        std::cout<< "Destructing MemoryMonitorTask and closing log file.\n"<<std::endl;
        logFile << "Destructing CpuMonitorTask and closing log file.\n" << std::endl;
        //logFile.flush();
        logFile.close();
    };
}

double parseLine(char* line){

    int i = strlen(line);
    line[i-3] = '\n';
    char* p = line;

    while(*p < '0' || *p > '9')p++;

    try {
        return std::stoi(std::string(p));
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Out of range: " << e.what() << std::endl;
    }
    return -1;
}

std::string parseProcessName(std::string& line){
    std::istringstream iss(line);
    std::string name;
    std::string label;

    iss >> label;

    getline(iss, name);
    size_t pos = name.find_first_not_of(" \t\n\r\f\v");
    if(pos != std::string::npos){
        name = name.substr(pos);
    }

    return name;
}


void MemoryMonitorTask::init(){

    FILE* file = fopen("/proc/self/status", "r");
    if(file == nullptr){

        std::cerr << "Error: Failed to open /proc/self/status" << std::endl;

        if(logFile.is_open()){
            logFile << "Memory Monitoring log file has been successfully opened" << std::endl;
        }
        return;
    }
    fclose(file);
}

void MemoryMonitorTask::runTask(){
    while(!isCancelled()){

        init();
        auto current_time = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(current_time);
        logFile << std::put_time(std::localtime(&time), "%F %T") << std::endl;
        logFile << "Running Memory Monitor Task..." << std::endl;

        double totalVirtualMem = getTotalVirtualMemory();
        double virtualMemUsed = getTotalVirtualMemoryUsed();
        double virtualMemUsedByProcess = getTotalVirtualMemoryUsedByProcess();
        double totalPhysMem = getTotalPhysicalMemory();
        double physMemUsed = getTotalPhysicalMemoryUsed();
        double physMemUsedByProcess = getTotalPhysicalMemoryUsedByProcess();

        logFile << "Total Virtual Memory: " << totalVirtualMem / (1024.0 * 1024.0) << " MB" << std::endl;
        logFile << "Virtual Memory Used: " << virtualMemUsed / (1024.0 * 1024.0) << " MB" << std::endl;
        logFile << "Virtual Memory Used by Process: " << virtualMemUsedByProcess / 1024.0 << " MB" << std::endl;
        logFile << "Total Physical Memory: " << totalPhysMem / (1024.0 * 1024.0) << " MB" << std::endl;
        logFile << "Physical Memory Used: " << physMemUsed / (1024.0 * 1024.0) << " MB" << std::endl;
        logFile << "Physical Memory Used by Process: " << physMemUsedByProcess / 1024.0 << " MB" << std::endl;

        const int num_of_process = 10; //modify this

        std::vector<ProcessInfo>top_n_sorted_processes = top_n_memory_processes(num_of_process);

        logFile << "Top " << num_of_process << " processes by memory usage:" << std::endl;
        for (const auto& process : top_n_sorted_processes) {
            logFile << "PID: " << process.pid << ", Name: " << process.name << ", Memory usage: " << process.mem_usage << " KB, Memory utilization: " << process.mem_usage_percentage << "%" << std::endl;
        }

        logFile << "Memory monitoring task iteration completed.\n\n" << std::endl;
        std::cout << "Memory monitoring task iteration completed.\n" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3)); // Suspend the task for 3 seconds

    };
}


double MemoryMonitorTask::getTotalVirtualMemory(){
    struct sysinfo virtMemInfo;
    sysinfo(&virtMemInfo);

    unsigned long totalVirtMem = virtMemInfo.totalram;
    totalVirtMem += virtMemInfo.totalswap;
    totalVirtMem *= virtMemInfo.mem_unit;
    
    return totalVirtMem;
}

double MemoryMonitorTask::getTotalVirtualMemoryUsed(){
    struct sysinfo virtMemInfo;
    sysinfo(&virtMemInfo);
    
    unsigned long totalVirtMemUsed = virtMemInfo.totalram - virtMemInfo.freeram;
    totalVirtMemUsed += virtMemInfo.totalswap - virtMemInfo.freeswap;
    totalVirtMemUsed *= virtMemInfo.mem_unit;

    return totalVirtMemUsed;
}

double MemoryMonitorTask::getTotalVirtualMemoryUsedByProcess(){
    FILE* file = fopen("/proc/self/status", "r");
    int virtMem = -1;
    char line[128];

    while(fgets(line,128,file) != NULL){
        if(strncmp("VmSize:", line, 7) == 0){
            virtMem = parseLine(line);
            break;
        }
    }
    fclose(file);
    return virtMem * 1024.0;
}

double MemoryMonitorTask::getTotalPhysicalMemory(){
    struct sysinfo physicalMemInfo;
    sysinfo(&physicalMemInfo);

    unsigned long totalPhysicalMemory = physicalMemInfo.totalram;
    totalPhysicalMemory *= physicalMemInfo.mem_unit;

    return totalPhysicalMemory;
}

double MemoryMonitorTask::getTotalPhysicalMemoryUsed(){
    struct sysinfo physicalMemInfo;
    sysinfo(&physicalMemInfo);

    unsigned long totalPhysicalMemUsed = physicalMemInfo.totalram - physicalMemInfo.freeram;
    totalPhysicalMemUsed *= physicalMemInfo.mem_unit;

    return totalPhysicalMemUsed;
}

double MemoryMonitorTask::getTotalPhysicalMemoryUsedByProcess(){
    FILE* file = fopen("/proc/self/status", "r");
    int phyMem = -1;
    char line[128];

    while(fgets(line, 128, file) != NULL){
        if(strncmp(line, "VmRSS:", 6) == 0){
            phyMem = parseLine(line);
            break;
        }
    }
    fclose(file);
    return phyMem * 1024.0;
}

std::vector<MemoryMonitorTask::ProcessInfo>MemoryMonitorTask::top_n_memory_processes(int n){
    std::vector<ProcessInfo> process_list;

    for(const auto& entry : std::filesystem::directory_iterator("/proc")){
        if(entry.is_directory()){
            const std::string is_pid = entry.path().filename().string();
            if(std::all_of(is_pid.begin(), is_pid.end(), ::isdigit)){
                int pid = stoi(is_pid);
                std::string status_file = entry.path().string() + "/status";
                std::ifstream status_read(status_file);
                std::string process_info;
                struct ProcessInfo pinfo;
                pinfo.pid = pid;
                bool is_user_task = false;
                while(std::getline(status_read, process_info)){
                    if(process_info.find("VmRSS:")==0){
                        int mem_usage = parseLine(process_info.data());
                        pinfo.mem_usage = mem_usage;
                        pinfo.mem_usage_percentage = (static_cast<double>(mem_usage) / getTotalPhysicalMemory()) * 100;
                        is_user_task = true;
                    }
                    if(process_info.find("Name:")==0){
                        pinfo.name = parseProcessName(process_info);
                    }
                }
                if(is_user_task){
                    process_list.push_back(pinfo);
                }

            }  
        }
    }
    std::sort(process_list.begin(), process_list.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.mem_usage_percentage > b.mem_usage_percentage;
    });

    if (process_list.size() > n) {
        process_list.resize(n);
    }
    return process_list;
}
