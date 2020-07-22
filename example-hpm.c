/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/hpm.h>

int main() {
  /* Enable module */
  if (metal_hpm_init() != 0) {
    return 1;
  }

  /* Set counter 3 to increment on 'event 15, class 0'
   *i.e. counts every 'JAL' instruction retired. */
  if (metal_hpm_set_event_mask(METAL_HPM_COUNTER_3,
                               METAL_HPM_EVENTID_15 | METAL_HPM_EVENTCLASS_0) == 0) {
    /* Read 'JAL' instruction retired count */
    if (metal_hpm_read_counter(METAL_HPM_COUNTER_3) == 0)
      return 2;
  }

  /* Set counter 4 to increment on 'event 14 class 0'
   *i.e. counts conditional branch retired. */
  if (metal_hpm_set_event_mask(METAL_HPM_COUNTER_4,
                               METAL_HPM_EVENTID_14 | METAL_HPM_EVENTCLASS_0) == 0) {
    /* Read conditional branch count */
    if (metal_hpm_read_counter(METAL_HPM_COUNTER_4) == 0)
      return 3;
  }
  /* Note that mcycle, mtime, minstret are enabled by default,
   * hence do not need to be explicitly set like done above.  */
  /* 'mtime' register is memory mapped into CLINT block.
   * Use CLINT APIs to access this register. */

  /* Read elapsed CPU cycles */
  if (metal_hpm_read_counter(METAL_HPM_CYCLE) == 0)
    return 4;

  /* Test enable / disable access APIs */
  if (metal_hpm_enable_access(METAL_HPM_CYCLE) != 0) {
    return 5;
  }

  if (metal_hpm_disable_access(METAL_HPM_CYCLE) != 0) {
    return 6;
  }

  return 0;
}
