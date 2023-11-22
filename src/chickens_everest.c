/* SPDX-License-Identifier: MIT */

#include "cpu_regs.h"
#include "uart.h"
#include "utils.h"
static void init_common_everest(void)
{
    reg_set(SYS_IMP_APL_HID12, BIT(46));
    if (is_ecore())
        reg_set(SYS_IMP_APL_EHID0, EHID0_BLI_UNK32);
    // "configure dummy cycles to work around incorrect temp sensor readings on
    // NEX power gating" (maybe)
    reg_mask(SYS_IMP_APL_HID13,
             HID13_POST_OFF_CYCLES_MASK | HID13_POST_ON_CYCLES_MASK | HID13_PRE_CYCLES_MASK |
                 HID13_GROUP0_FF1_DELAY_MASK | HID13_GROUP0_FF2_DELAY_MASK |
                 HID13_GROUP0_FF3_DELAY_MASK | HID13_GROUP0_FF4_DELAY_MASK |
                 HID13_GROUP0_FF5_DELAY_MASK | HID13_GROUP0_FF6_DELAY_MASK |
                 HID13_GROUP0_FF7_DELAY_MASK | HID13_RESET_CYCLES_MASK,
             HID13_POST_OFF_CYCLES(4) | HID13_POST_ON_CYCLES(5) | HID13_PRE_CYCLES(1) |
                 HID13_GROUP0_FF1_DELAY(4) | HID13_GROUP0_FF2_DELAY(4) | HID13_GROUP0_FF3_DELAY(4) |
                 HID13_GROUP0_FF4_DELAY(4) | HID13_GROUP0_FF5_DELAY(4) | HID13_GROUP0_FF6_DELAY(4) |
                 HID13_GROUP0_FF7_DELAY(4) | HID13_RESET_CYCLES(0));

    /* Figure out how this really changed later */
    /*   reg_mask(SYS_IMP_APL_HID26, HID26_GROUP1_OFFSET_MASK | HID26_GROUP2_OFFSET_MASK,
                 HID26_GROUP1_OFFSET(26) | HID26_GROUP2_OFFSET(31));*/
    msr(SYS_IMP_APL_HID26, HID26_GROUP1_OFFSET(0xF88F65588LL) | HID26_GROUP2_OFFSET(0x3F28));
    /*    reg_mask(SYS_IMP_APL_HID27, BIT(40)HID27_GROUP3_OFFSET_MASK, HID27_GROUP3_OFFSET(31)); */
    /* ditto on this */
    msr(SYS_IMP_APL_HID27, BIT(0) | BIT(4) | (0x2B << 8) | BIT(16) | BIT(20) | BIT(24) | BIT(28) |
                               BIT(32) | BIT(36) | BIT(40));
    reg_set(SYS_IMP_APL_HID16, BIT(54));
    /* This is new to M3 and i have no idea what it is yet */
    reg_clr(s3_0_c15_c2_4, BIT(0) | BIT(1) | BIT(16) | BIT(17) | BIT(18) | BIT(22));
}

void init_t6031_everest(int rev)
{
    UNUSED(rev);
    uart_puts("Everest\n");
    init_common_everest();
    reg_set(SYS_IMP_APL_HID3, BIT(63));
    // The T6031 code seems to have changed the pcie throttle mask
    reg_mask(SYS_IMP_APL_HID3, GENMASK(ULONG(62), ULONG(56)), BIT(60) | BIT(59) | BIT(58));
    reg_clr(SYS_IMP_APL_HID3, BIT(4));
    reg_set(SYS_IMP_APL_HID18, HID18_AVL_UNK27 | HID18_AVL_UNK29);
    reg_set(SYS_IMP_APL_HID16, HID16_AVL_UNK12);

    reg_mask(SYS_IMP_APL_HID5, HID5_BLZ_UNK_19_18_MASK, HID5_BLZ_UNK19);
}
