#include <Poco/TaskManager.h>
#include "CpuMonitorTask.h"
#include "MemoryMonitorTask.h"
#include <chrono>
#include <thread>

int main() {
    Poco::TaskManager tm;
    CpuMonitorTask* CPU_task = new CpuMonitorTask("CPU Monitor Task");
    MemoryMonitorTask* Memory_task = new MemoryMonitorTask("Memory Monitor Task");
    tm.start(CPU_task);
    tm.start(Memory_task);

    std::this_thread::sleep_for(std::chrono::seconds(20));

    tm.cancelAll();
    tm.joinAll();
    return 0;
}
