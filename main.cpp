#include <thread>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <unistd.h> // For getpid() and fork()
#include <sys/types.h>
#include <sys/wait.h> // For waitpid()
#include "performant_thread.h"

// Struct representing sensor readings
struct SensorReadings {
    alignas(64) int8_t temperature;  // Aligning to cache lines to avoid false sharing
    alignas(64) int8_t humidity;
    alignas(64) int8_t pressure;
    alignas(64) int8_t light;
    alignas(64) int8_t co2;
    alignas(64) int8_t aqi;
};

struct thread_info {
    void (*thread_func)(uint32_t, uint8_t *, int8_t *) = nullptr;
    uint32_t threadIdx = -1;
    uint8_t *data_addr = nullptr;
    int8_t *data = nullptr;
};

std::vector<thread_info> _threads;
SensorReadings readings;
pid_t perf_pid = -1; // Store the process ID for perf

int work_init() {
    std::cout << "Allocating & Initializing Memory" << std::endl;

    int thread_count = 6;
    for (int i = 0; i < thread_count; i++) {
        thread_info thread_info;
        thread_info.thread_func = performant_thread_run;
        thread_info.threadIdx = i;

        // Memory allocation with alignment to avoid false sharing
        uint8_t *thread_data = (uint8_t *)aligned_alloc(64, TOTAL_SIZE);  // Align memory to cache lines
        madvise(thread_data, TOTAL_SIZE, MADV_HUGEPAGE);
        
        for (long unsigned int j = 0; j < TOTAL_SIZE; j++) {
            thread_data[j] = rand() % 256;
        }
        thread_info.data_addr = thread_data;

        // Assign sensor reading to each thread
        switch(i) {
            case 0:
                thread_info.data = &readings.temperature;
                break;
            case 1:
                thread_info.data = &readings.humidity;
                break;
            case 2:
                thread_info.data = &readings.pressure;
                break;
            case 3:
                thread_info.data = &readings.light;
                break;
            case 4:
                thread_info.data = &readings.co2;
                break;
            case 5:
                thread_info.data = &readings.aqi;
                break;
            default:
                perror("Error, no need for this thread");
        }
        _threads.push_back(thread_info);
    }
    return _threads.size();
}



void work_run() {
    std::cout << "Starting Threads for Running" << std::endl;
    std::vector<std::thread> threads;

    // start_profiling();  // Start profiling before threads
    for (long unsigned int i = 0; i < _threads.size(); i++) {
        
        threads.push_back(std::thread(_threads[i].thread_func, _threads[i].threadIdx, _threads[i].data_addr, _threads[i].data));
    }

    for (long unsigned int i = 0; i < threads.size(); i++) {

        threads[i].join();  // Wait for all threads to finish
    }

    // stop_profiling();  // Stop profiling after all threads complete
}

int main(void) {
    work_init();
    work_run();

    std::cout << "Final Sensor Readings:\n";
    std::cout << "Temperature: " << static_cast<int>(readings.temperature) << "\n";
    std::cout << "Humidity: " << static_cast<int>(readings.humidity) << "\n";
    std::cout << "Pressure: " << static_cast<int>(readings.pressure) << "\n";
    std::cout << "Light: " << static_cast<int>(readings.light) << "\n";
    std::cout << "CO2: " << static_cast<int>(readings.co2) << "\n";
    std::cout << "AQI: " << static_cast<int>(readings.aqi) << "\n";
}

// Allocating & Initializing Memory
// Starting Threads for Running
// TheadIdx: 2 completed, time was 5.506130719.
// TheadIdx: 0 completed, time was 5.507757937.
// TheadIdx: 5 completed, time was 5.510353263.
// TheadIdx: 4 completed, time was 5.511734870.
// TheadIdx: 1 completed, time was 5.513642635.
// TheadIdx: 3 completed, time was 5.518212005.
// Final Sensor Readings:
// Temperature: 88
// Humidity: 82
// Pressure: 98
// Light: 53
// CO2: 64
// AQI: 80

//  Performance counter stats for './main':

//          33,156.75 msec task-clock                #    5.910 CPUs utilized          
//                112      context-switches          #    0.003 K/sec                  
//                 22      cpu-migrations            #    0.001 K/sec                  
//              1,708      page-faults               #    0.052 K/sec                  
//    102,222,571,365      cycles                    #    3.083 GHz                    
//    313,275,604,652      instructions              #    3.06  insn per cycle         
//     20,383,375,514      branches                  #  614.758 M/sec                  
//         36,511,673      branch-misses             #    0.18% of all branches        
//    100,621,524,252      L1-dcache-loads           # 3034.722 M/sec                  
//         27,931,493      L1-dcache-load-misses     #    0.03% of all L1-dcache accesses
//         17,890,749      LLC-loads                 #    0.540 M/sec                  
//          2,063,900      LLC-load-misses           #   11.54% of all LL-cache accesses

//        5.610184845 seconds time elapsed

//       33.153703000 seconds user
//        0.003999000 seconds sys

