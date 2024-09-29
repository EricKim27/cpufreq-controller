#include "proccon.h"
#include <linux/cpufreq.h>

void limit_freq(unsigned int max, unsigned int min)
{
    unsigned int cpu;
    struct cpufreq_policy *plcy;
    for_each_online_cpu(cpu) {
        plcy = cpufreq_cpu_get(cpu);
        if (!plcy) {
            pr_err("Failed to get CPU policy for CPU %u\n", cpu);
            return;
        }
        
        pr_info("Current policy for CPU %u: min=%u, max=%u\n", cpu, plcy->min, plcy->max);
        pr_info("Dest frequency: max %d, min %d", max, min);
        if(max != plcy->cpuinfo.max_freq || min != plcy->cpuinfo.min_freq) {
            plcy->cpuinfo.max_freq = max;
            plcy->cpuinfo.min_freq = min;
            cpufreq_update_policy(cpu);
            cpufreq_cpu_put(plcy);
            
            plcy = cpufreq_cpu_get(cpu);
            if (!plcy) {
                pr_err("Failed to re-fetch CPU policy for CPU %u\n", cpu);
                return;
            }
            pr_info("Updated policy for CPU %u: min=%u, max=%u\n", cpu, plcy->min, plcy->max);
        } else {
            cpufreq_cpu_put(plcy);
        }
    }
}