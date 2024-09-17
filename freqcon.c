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
        if(max != plcy->cpuinfo.max_freq || min != plcy->cpuinfo.min_freq) {
            plcy->max = max;
            plcy->min = min;
            cpufreq_update_policy(cpu);
        }
        cpufreq_cpu_put(plcy);
    }
}