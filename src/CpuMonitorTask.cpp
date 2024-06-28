#include "CpuMonitorTask.h"
#include <Poco/TaskManager.h>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <thread>

unsigned long long CpuMonitorTask::lastTotalUser = 0, CpuMonitorTask::lastTotalUserLow = 0, CpuMonitorTask::lastTotalSys = 0, CpuMonitorTask::lastTotalIdle = 0;

CpuMonitorTask::CpuMonitorTask(const std::string& name) : Poco::Task(name), log_file("cpu_monitor.log") {
    std::cout << "\n-------------Initializing CpuMonitorTask-------------\n" << std::endl;
    logFile.open(log_file, std::ios::out | std::ios::app);

    if (logFile.is_open()) {
        //std::cout << "Log file opened successfully: " << log_file << std::endl;
        logFile << "Log file for CPU monitoring has been initialized\n" << std::endl;

        init();
        double current_CPU_utilization = getCurrentValue();
        if (current_CPU_utilization == -1.0) {
            std::cerr << "Error: Initial CPU utilization could not be determined" << std::endl;
            logFile << "Error: Initial CPU utilization could not be determined" << std::endl;
            current_CPU_utilization = 0.0; // Fallback to 0 if the initial value is invalid
        }
    } else {
        std::cerr << "Error: Failed to open log file: " << log_file << std::endl;
    }
}

CpuMonitorTask::~CpuMonitorTask() {
    if (logFile.is_open()) {
        
        std::cout<< "Destructing CpuMonitorTask and closing log file.\n"<<std::endl;
        logFile << "Destructing CpuMonitorTask and closing log file.\n";
        //logFile.flush();
        logFile.close();
    }
}

void CpuMonitorTask::runTask(){
    while (!isCancelled()) {
        auto current_time = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(current_time);
        logFile << std::put_time(std::localtime(&time), "%F %T") << std::endl;
        logFile << "Running CPU monitoring task..." << std::endl;

        double current_CPU_utilization = getCurrentValue();
        if (current_CPU_utilization == -1.0) {
            logFile << "Error: Failed to get current CPU utilization" << std::endl;
            current_CPU_utilization = 0.0; // Fallback to 0 if the current value is invalid
        }
        logFile << "CPU utilization: " << current_CPU_utilization << "%\n" << std::endl;

        const int process_num = 10; // number of processes you want to store in the log file

        std::vector<ProcessInfo> top_sorted_processes = top_n_cpu_processes(process_num); // calculate top 10 processes

        logFile << "Top " << process_num << " processes by CPU usage:" << std::endl;
        for (const auto& process : top_sorted_processes) {
            logFile << "PID: " << process.pid << ", Name: " << process.name << ", CPU usage time: " << process.cpu_usage << " seconds" << ", CPU utilization: " << process.cpu_usage_percentage << "%" << std::endl;
        }

        logFile << "CPU monitoring task iteration completed.\n\n" << std::endl;
        std::cout << "CPU monitoring task iteration completed.\n" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3)); // Suspend the task for 3 seconds
    }
}

void CpuMonitorTask::init() {
    FILE* file = fopen("/proc/stat", "r");
    if (file == nullptr) {
        std::cerr << "Error: Failed to open /proc/stat" << std::endl;
        if (logFile.is_open()) {
            logFile << "Error: Failed to open /proc/stat" << std::endl;
        }
        return;
    }
    fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow, &lastTotalSys, &lastTotalIdle);
    fclose(file);
}

double CpuMonitorTask::getCurrentValue() {
    double percent;
    FILE* file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    file = fopen("/proc/stat", "r");
    if (fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow, &totalSys, &totalIdle) != 4) {
        std::cerr << "Error: Failed to read /proc/stat" << std::endl;
        fclose(file);
        return 0;
    }
    fclose(file);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow || totalSys < lastTotalSys || totalIdle < lastTotalIdle) {
        percent = -1.0;
    } else {
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) + (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    return percent;
}

double CpuMonitorTask::sysUpTime() {
    std::ifstream file("/proc/uptime");
    double upTime, idleTime;
    file >> upTime >> idleTime;
    return upTime;
}

std::vector<CpuMonitorTask::ProcessInfo> CpuMonitorTask::top_n_cpu_processes(int n) {
    std::vector<ProcessInfo> process_list;

    for (const auto& entry : std::filesystem::directory_iterator("/proc")) {
        if (entry.is_directory()) {
            const std::string is_pid = entry.path().filename().string();
            if (std::all_of(is_pid.begin(), is_pid.end(), ::isdigit)) {
                int pid = std::stoi(is_pid);
                std::string stat_file = entry.path().string() + "/stat";
                std::ifstream stat_read(stat_file);
                std::string process_info;

                if (stat_read.is_open() && std::getline(stat_read, process_info)) {
                    std::istringstream process_stream(process_info);
                    std::vector<std::string> stats((std::istream_iterator<std::string>(process_stream)), std::istream_iterator<std::string>());

                    if (stats.size() >= 22) {
                        unsigned long user_time = std::stoull(stats[13]);
                        unsigned long kernel_time = std::stoull(stats[14]);
                        unsigned long processStartTime = std::stoull(stats[21]);
                        double systemUpTime = sysUpTime();
                        double seconds = systemUpTime - (processStartTime / (double)sysconf(_SC_CLK_TCK));
                        double user_and_kernel_time = user_time + kernel_time;
                        double cpu_usage_percentage = (user_and_kernel_time / (double)sysconf(_SC_CLK_TCK)) * 100 / seconds;
                        process_list.push_back({pid, stats[1], user_and_kernel_time, cpu_usage_percentage});
                    }
                }
            }
        }
    }

    std::sort(process_list.begin(), process_list.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.cpu_usage_percentage > b.cpu_usage_percentage;
    });

    if (process_list.size() > n) {
        process_list.resize(n);
    }
    return process_list;
}
