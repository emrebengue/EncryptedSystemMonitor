#pragma once

#include <Poco/Task.h>
#include <fstream>
#include <vector>
#include <string>

class MemoryMonitorTask : public Poco::Task{

public:
	MemoryMonitorTask(const std::string& name);
	~MemoryMonitorTask();

	void runTask() override;
	void init();
	double getTotalVirtualMemory();
	double getTotalVirtualMemoryUsed();
	double getTotalVirtualMemoryUsedByProcess();
	double getTotalPhysicalMemory();
	double getTotalPhysicalMemoryUsed();
	double getTotalPhysicalMemoryUsedByProcess();

private:

	std::string log_file;
	std::ofstream logFile;
	
	struct ProcessInfo{

		int pid;
		std::string name;
		double mem_usage;
		double mem_usage_percentage;
	};

	std::vector<ProcessInfo>top_n_memory_processes(int n);

	static int parseString(char* line);
};
