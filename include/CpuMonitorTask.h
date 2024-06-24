#pragma once

#include <Poco/Task.h>
#include <fstream>
#include <vector>
#include <string>

class CpuMonitorTask : public Poco::Task{

public:
	CpuMonitorTask(const std::string& name);
	~CpuMonitorTask();

	void runTask() override;

private:

	std::string log_file;
	std::ofstream logFile;

	void init();
	double getCurrentValue();

	struct ProcessInfo {
		int pid;
		std::string name;
		double cpu_usage;
		double cpu_usage_percentage;
	};

	std::vector<ProcessInfo>top_n_cpu_processes(int n);

	double sysUpTime();

	static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;
};
