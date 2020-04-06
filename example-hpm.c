/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/cpu.h>
#include <metal/hpm.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

int main() {
  unsigned long long count;
  struct metal_cpu *cpu;

  /* Get CPU device handle. */
  cpu = metal_cpu_get(metal_cpu_get_current_hartid());

  /* Enable module */
  if (metal_hpm_init(cpu) != 0) {
    return 1;
  }

  /* Set counter 3 to increment on 'event 15, class 0'
   *i.e. counts every 'JAL' instruction retired. */
  if (metal_hpm_set_event(cpu, METAL_HPM_COUNTER_3,
                                METAL_HPM_EVENTID_15 |
                                    METAL_HPM_EVENTCLASS_0) != 0) {
    return 2;
  }

  /* Note that mcycle, mtime, minstret are enabled by default,
   * hence do not need to be explicitly set like done above.  */
  /* 'mtime' register is memory mapped into CLINT block.
   * Use CLINT APIs to access this register. */

  /* Set counter 4 to increment on 'events 8,9 class 2'
   *i.e. counts I-cache miss and Memory mapped I/O access */
  if (metal_hpm_set_event(cpu, METAL_HPM_COUNTER_4,
                                METAL_HPM_EVENTID_9 |
                                    METAL_HPM_EVENTID_8 |
                                    METAL_HPM_EVENTCLASS_2) != 0) {
    return 3;
  }

  /* Read elapsed CPU cycles */
  count = metal_hpm_read_counter(cpu, METAL_HPM_CYCLE);

  if (count == 0)
    return 4;

  /* Read 'JAL' instruction retired count */
  count = metal_hpm_read_counter(cpu, METAL_HPM_COUNTER_3);

  if (count == 0)
    return 5;

  /* Read I-cache miss / Memory mapped I/O access count */
  count = metal_hpm_read_counter(cpu, METAL_HPM_COUNTER_4);

  if (count == 0)
    return 6;

  return 0;
}
