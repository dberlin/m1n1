/* SPDX-License-Identifier: MIT */

#include "arm_cpu_regs.h"
#include "types.h"

/* ARM extensions */
#define ESR_EC_IMPDEF      0b111111
#define ESR_ISS_IMPDEF_MSR 0x20

#define SYS_IMP_APL_ACTLR_EL12 sys_reg(3, 6, 15, 14, 6)

#define SYS_IMP_APL_AMX_CTL_EL1  sys_reg(3, 4, 15, 1, 4)
#define SYS_IMP_APL_AMX_CTL_EL2  sys_reg(3, 4, 15, 4, 7)
#define SYS_IMP_APL_AMX_CTL_EL12 sys_reg(3, 4, 15, 4, 6)

#define AMX_CTL_EN     BIT(63)
#define AMX_CTL_EN_EL1 BIT(62)

#define SYS_IMP_APL_CNTVCT_ALIAS_EL0 sys_reg(3, 4, 15, 10, 6)

/* HID registers */
#define SYS_IMP_APL_HID0                sys_reg(3, 0, 15, 0, 0)
#define HID0_FETCH_WIDTH_DISABLE        BIT(28)
#define HID0_CACHE_FUSION_DISABLE       BIT(36)
#define HID0_SAME_PG_POWER_OPTIMIZATION BIT(45)

#define SYS_IMP_APL_EHID0 sys_reg(3, 0, 15, 0, 1)
#define EHID0_BLI_UNK32   BIT(32)

#define SYS_IMP_APL_HID1                       sys_reg(3, 0, 15, 1, 0)
#define HID1_RSS_FORCE_NS_ISSUE                BIT(0)
#define HID1_RSS_FORCE_NS_SPR_RD               BIT(1)
#define HID1_RSS_DIS_NS_STREAMING              BIT(2)
#define HID1_REDIR_FORCE_SPR_SYNC              BIT(3)
#define HID1_LSP_DISABLE                       BIT(4)
#define HID1_FORCE_SSBS_ORDERING               BIT(5)
#define HID1_LSP_AGE_OUT_INTERVAL              BIT(6)
#define HID1_DIS_INT_ZCM                       BIT(8)
#define HID1_DIS_NEON_ZCM                      BIT(9)
#define HID1_FORCE_SPR_L3_CLK_ON               BIT(10)
#define HID1_DIS_MULT_RETIRE                   BIT(11)
#define HID1_DIS_MULT_INDIR_BR_RETIRE          BIT(12)
#define HID1_DIS_BR_DISP_SYS_RSLVD_PTR         BIT(13)
#define HID1_DIS_CMP_BR_FUSION                 BIT(14)
#define HID1_FORCE_NEX_L3_CLK_ON               BIT(15)
#define HID1_INTDISP_LIM_MODE                  BIT(16)
#define HID1_NEON_DISP_LIM_MODE                BIT(17)
#define HID1_FORCE_PRECISE_NEON_GRP_MODE       BIT(18)
#define HID1_RCC_FORCE_ALL_MDR_L6_CLKS_ON      BIT(19)
#define HID1_RCC_FORCE_ALL_MDR_L3_CLKS_ON      BIT(20)
#define HID1_RCC_DIS_STALL_INACTIVE_MDR_CTL    BIT(21)
#define HID1_ZCL_RF_RESTART_THRESHOLD(x)       ((ULONG(x)) << 22)
#define HID1_ZCL_RF_RESTART_THRESHOLD_MASK     GENMASK(23, 22)
#define HID1_DIS_SPEC_MDSB_INVL_ROB_FLUSH      BIT(24)
#define HID1_DIS_LSP_FLUSH_WITH_CONTEXT_SWITCH BIT(25)
#define HID1_DIS_WFE                           BIT(26)
#define HID1_DIS_WFI                           BIT(27)
#define HID1_EN_CS_RFOR_SP_SEL_IMM             BIT(28)
#define HID1_RCC_FORCE_CHKPT_L3_CLKS_ON        BIT(29)
#define HID1_DISABLE_CHKPT                     BIT(30)
#define HID1_EN_SICILY_SDSB                    BIT(31)
#define HID1_FORCE_WFE                         BIT(32)
#define HID1_FORCE_GRP_BNDRY_ON_INST_END       BIT(33)
#define HID1_FORCE_GRP_BNDRY_ON_UCODE_END      BIT(34)
#define HID1_DIS_LCL_TIME_BASE                 BIT(35)
#define HID1_EN_CS_RFOR_PAN_IMM                BIT(36)
#define HID1_EN_CS_RFOR_AMXT_IMM               BIT(37)
#define HID1_ZCL_SP_MISPREDICT_THRESHOLD(x)    ((ULONG(x)) << 38)
#define HID1_ZCL_SP_MISPREDICT_THRESHOLD_MASK  GENMASK(39, 38)
#define HID1_ZCL_SP_RESTART_THRESHOLD(x)       ((ULONG(x)) << 40)
#define HID1_ZCL_SP_RESTART_THRESHOLD_MASK     GENMASK(41, 40)
#define HID1_ZCL_RF_MISPREDICT_THRESHOLD(x)    ((ULONG(x)) << 42)
#define HID1_ZCL_RF_MISPREDICT_THRESHOLD_MASK  GENMASK(43, 42)
#define HID1_CONSERVATIVE_SIQ                  BIT(44)
#define HID1_DIS_AES_FUSION                    BIT(45)
#define HID1_RSS_DIS_MULTIPLE_CPM_IN_FLIGHT    BIT(46)
#define HID1_DIS_HW_TRACE_SYNC                 BIT(47)
#define HID1_DIS_HW_TRACE_SYNC_ON_UARCH_REDIR  BIT(48)
#define HID1_DIS_MSR_SPEC_DAIF                 BIT(49)
#define HID1_DIS_MRS_SPEC_DAIF                 BIT(50)
#define HID1_DIS_MSR_SPEC_NON_DAIF             BIT(51)
#define HID1_DIS_MRS_SPEC_NON_DAIF             BIT(52)
#define HID1_DIS_DBG_PROG_OVRD                 BIT(53)
#define HID1_TRAP_SMC                          BIT(54)
#define HID1_ENABLE_MDSB_STALL_PIPELINE_ECO    BIT(58)
#define HID1_ENABLE_BR_KILL_LIMIT              BIT(60)
#define HID1_CPMU_DEBUG_OVF                    BIT(61)
#define HID1_ZCL_RF_GNUM_DIST_MODE             BIT(62)

#define SYS_IMP_APL_EHID1                        sys_reg(3, 0, 15, 1, 1)
#define EHID1_RSS_FORCE_NS_ISSUE                 BIT(0)
#define EHID1_RSS_FORCE_NS_SPR_RD                BIT(1)
#define EHID1_RSS_DIS_NS_STREAMING               BIT(2)
#define EHID1_REDIR_FORCE_SPR_SYNC               BIT(3)
#define EHID1_LSP_MODE(x)                        ((ULONG(X)) << 4)
#define EHID1_LSP_MODE_MASK                      GENMASK(5, 4)
#define EHID1_LSP_AGE_OUT_INTERVAL(x)            ((ULONG(X)) << 6)
#define EHID1_LSP_AGE_OUT_INTERVAL_MASK          GENMASK(7, 6)
#define EHID1_RSS_DIS_MULTIPLE_CPM_IN_FLIGHT     BIT(8)
#define EHID1_EN_CS_RFOR_AMXSETCLR               BIT(9)
#define EHID1_FORCE_SPR_L3_CLK_ON                BIT(10)
#define EHID1_DIS_MULT_RETIRE                    BIT(11)
#define EHID1_DIS_MULT_INDIR_BR_RETIRE           BIT(12)
#define EHID1_DIS_BR_SYS_RSLV_PTR                BIT(13)
#define EHID1_DIS_CMP_BR_FUSION                  BIT(14)
#define EHID1_FORCE_NEX_L3_CLK_ON                BIT(15)
#define EHID1_DIS_LCL_TIME_BASE                  BIT(16)
#define EHID1_NEON_DISP_LIM_MODE                 BIT(17)
#define EHID1_EN_CS_RFOR_PAN_IMM                 BIT(18)
#define EHID1_RCC_FORCE_ALL_MDR_L6_CLKS_ON       BIT(19)
#define EHID1_RCC_FORCE_ALL_MDR_L3_CLKS_ON       BIT(20)
#define EHID1_RCC_DIS_STALL_INACTIVE_MDR_CTL     BIT(21)
#define EHID1_RCC_FORCE_ALL_IEX_L6_CLKS_ON       BIT(22)
#define EHID1_RCC_FORCE_ALL_IEX_L3_CLKS_ON       BIT(23)
#define EHID1_RCC_DIS_STALL_INACTIVE_IEX_CTL     BIT(24)
#define EHID1_DIS_LSP_FLUSH_WITH_CONTEXT_SWITCH  BIT(25)
#define EHID1_DIS_WFE                            BIT(26)
#define EHID1_DIS_WFI                            BIT(27)
#define EHID1_EN_CS_RFOR_SP_SEL_IMM              BIT(28)
#define EHID1_DIS_LSP_TRAIN_FILTER               BIT(29)
#define EHID1_DIS_MSR_SPEC_DAIF                  BIT(30)
#define EHID1_DISABLE_CMP_BR_ACROSS_GRP          BIT(31)
#define EHID1_FORCE_WFE                          BIT(32)
#define EHID1_FORCE_GRP_BNDRY_ON_INST_END        BIT(33)
#define EHID1_FORCE_GRP_BNDRY_ON_UCODE_END       BIT(34)
#define EHID1_LSP_CNT_INIT_VALUE(x)              ((ULONG(X)) << 35)
#define EHID1_LSP_CNT_INIT_VALUE_MASK            GENMASK(37, 35)
#define EHID1_LSP_CNT_RETRAIN_VALUE(x)           ((ULONG(X)) << 38)
#define EHID1_LSP_CNT_RETRAIN_VALUE_MASK         GENMASK(40, 38)
#define EHID1_LSP_CNT_INC_VALUE(x)               ((ULONG(X)) << 41)
#define EHID1_LSP_CNT_INC_VALUE_MASK             GENMASK(43, 41)
#define EHID1_LSP_CNT_RPLY_INIT_VALUE(x)         ((ULONG(X)) << 44)
#define EHID1_LSP_CNT_RPLY_INIT_VALUE_MASK       GENMASK(46, 44)
#define EHID1_DIS_MRS_SPEC_DAIF                  BIT(47)
#define EHID1_PCSAMPLE_ALLRETIRES                BIT(48)
#define EHID1_DIS_AES_FUSION                     BIT(49)
#define EHID1_DIS_ZCM                            BIT(50)
#define EHID1_DIS_RETIRE_GREATER_THAN_TWO_GROUPS BIT(51)
#define EHID1_LFSR_SEED(x)                       ((ULONG(X)) << 52)
#define EHID1_LFSR_SEED_MASK                     GENMASK(58, 52)
#define EHID1_EN_LFSR_STALL_RS01                 BIT(59)
#define EHID1_EN_LFSR_STALL_RS4                  BIT(60)
#define EHID1_CPMU_DEBUG_OVF                     BIT(61)
#define EHID1_EN_LFSR_STALL_RS6                  BIT(62)
#define EHID1_EN_LFSR                            BIT(63)

#define SYS_IMP_APL_HID3                  sys_reg(3, 0, 15, 3, 0)
#define HID3_DISABLE_ARBITER_FIX_BIF_CRD  BIT(44)
#define HID3_DEV_PCIE_THROTTLE_LIMIT_MASK GENMASK(62, 57)
#define HID3_DEV_PCIE_THROTTLE_LIMIT(x)   ((ULONG(x)) << 57)
#define HID3_DEV_PCIE_THROTTLE_ENABLE     BIT(63)

#define SYS_IMP_APL_HID4                                         sys_reg(3, 0, 15, 4, 0)
#define HID4_INV_CORE_CLK_OBS_TO_SOC                             BIT(0)
#define HID4_DISABLE_STNT_WIDGET                                 BIT(1)
#define HID4_DISABLE_SW_PRELOAD                                  BIT(2)
#define HID4_DISABLE_ST_LD_REDIR_MULTI_HIT_CHK                   BIT(3)
#define HID4_FORCE_CPU_OLDEST_IN_ORDER                           BIT(4)
#define HID4_FORCE_S_STEP_STORE_COMMIT                           BIT(5)
#define HID4_FORCE_S_STEP_PASS2_TO_CIF                           BIT(6)
#define HID4_FORCE_ST_LNCH_NO_OLDER_LD                           BIT(7)
#define HID4_DISABLE_LD_RTR_AHEAD_OLDER_ST                       BIT(8)
#define HID4_DISABLE_SPEC_LS_REDIRECT                            BIT(9)
#define HID4_DISABLE_SPEC_LDREX_PLAN_B                           BIT(10)
#define HID4_DISABLE_DC_MVA                                      BIT(11)
#define HID4_ENABLE_CORE_CLK_OBS_TO_SOC                          BIT(12)
#define HID4_RCC_FORCE_ALL_LSI_L6_CLKS_ON                        BIT(13)
#define HID4_RCC_FORCE_ALL_LSU_L3_CLKS_ON                        BIT(14)
#define HID4_RCC_DIS_STALL_INACTIVE_LSU_CTL                      BIT(15)
#define HID4_DIS_ANTI_LIVELOCK_ON_LOCK_VICTIMIZATION             BIT(16)
#define HID4_CT0_ANTI_LIVELOCK_ON_LOCK_VICTIMIZATION(x)          ((ULONG(x)) << 17)
#define HID4_CT0_ANTI_LIVELOCK_ON_LOCK_VICTIMIZATION_MASK        GENMASK(19, 17)
#define HID4_CT1_ANTI_LIVELOCK_ON_LOCK_VICTIMIZATION(x)          ((ULONG(x)) << 20)
#define HID4_CT1_ANTI_LIVELOCK_ON_LOCK_VICTIMIZATION_MASK        GENMASK(22, 20)
#define HID4_CT2_ANTI_LIVELOCK_ON_LOCK_VICTIMIZATION(x)          ((ULONG(x)) << 23)
#define HID4_CT2_ANTI_LIVELOCK_ON_LOCK_VICTIMIZATION_MASK        GENMASK(25, 23)
#define HID4_CT0_ANTI_LIVELOCK_ON_OLDEST_LD_ST_REPLAY(x)         ((ULONG(x)) << 26)
#define HID4_CT0_ANTI_LIVELOCK_ON_OLDEST_LD_ST_REPLAY_MASK       GENMASK(28, 26)
#define HID4_CT0_ANTI_LIVELOCK_ON_LS_ISS_STALL_ON_LS_REPLAY(x)   ((ULONG(x)) << 29)
#define HID4_CT0_ANTI_LIVELOCK_ON_LS_ISS_STALL_ON_LS_REPLAY_MASK GENMASK(31, 29)
#define HID4_DIS_ST_RTR_AHEAD_OLDER_LD                           BIT(32)
#define HID4_DIS_SPEC_LNCH_READ                                  BIT(33)
#define HID4_FORCE_NS_ORD_LD_REQ_NO_OLDER_ST                     BIT(34)
#define HID4_NON_CNTG_THRESH(x)                                  ((ULONG(x)) << 35)
#define HID4_NON_CNTG_THRESH_MASK                                GENMASK(36, 35)
#define HID4_FORCE_YNG_LD_FLUSH_ON_BAR_OP                        BIT(37)
#define HID4_FORCE_YNG_LD_FLUSH_ON_LD_ACQ                        BIT(38)
#define HID4_FORCE_NS_ORD_LD_REQ_NO_OLDER_LD                     BIT(39)
#define HID4_CNF_CNTR_THRESH(x)                                  ((ULONG(x)) << 40)
#define HID4_CNF_CNTR_THRESH_MASK                                GENMASK(41, 40)
#define HID4_FORCE_BAR_LNCH_NO_OLDER_ST                          BIT(42)
#define HID4_DISABLE_PWR_SAVE_LNBUF_CLK_OFF                      BIT(43)
#define HID4_DISABLE_DC_SW_L2_OPS                                BIT(44)
#define HID4_FORCE_NS_ORD_LD_REQ_NO_IN_PIPE_ORD_LD               BIT(45)
#define HID4_ENABLE_LFSR_STALL_STQ_RTR(x)                        ((ULONG(x)) << 46)
#define HID4_ENABLE_LFSR_STALL_STQ_RTR_MASK                      GENMASK(47, 46)
#define HID4_ENABLE_LFSR_STALL_LDQ_RTR                           BIT(48)
#define HID4_ENABLE_LFSR_STALL_LOAD_PIPE2_ISSUE                  BIT(49)
#define HID4_ENABLE_LFSR_STALL_PASS2_LAUNCH                      BIT(50)
#define HID4_ENABLE_LFSR_STALL_LOAD_STORE_PIPE1_ISSUE            BIT(51)
#define HID4_ENABLE_LFSR_STALL_LOAD_STORE_PIPE0_ISSUE            BIT(52)
#define HID4_ENABLE_LFSR_STALL_STQ_REPLAY                        BIT(53)
#define HID4_ENABLE_LFSR_STALL_LDQ_REPLAY                        BIT(54)
#define HID4_ENABLE_LFSR_STALL_SMB_DRAIN                         BIT(55)
#define HID4_LFSR_SEED(x)                                        ((ULONG(x)) << 56)
#define HID4_LFSR_SEED_MASK                                      GENMASK(62, 56)
#define HID4_ENABLE_LFSR                                         BIT(63)

#define SYS_IMP_APL_EHID4                                         sys_reg(3, 0, 15, 4, 1)
#define EHID4_DISABLE_HW_PREF_LD                                  BIT(0)
#define EHID4_DISABLE_HW_PREF_ST                                  BIT(1)
#define EHID4_DISABLE_SW_PRELOAD                                  BIT(2)
#define EHID4_DISABLE_ST_LD_REDIR_MULTI_HIT_CHK                   BIT(3)
#define EHID4_FORCE_CPU_OLDEST_IN_ORDER                           BIT(4)
#define EHID4_FORCE_S_STEP_STORE_COMMIT                           BIT(5)
#define EHID4_FORCE_S_STEP_PASS2_TO_CIF                           BIT(6)
#define EHID4_FORCE_ST_LNCH_NO_OLDER_LD                           BIT(7)
#define EHID4_DISABLE_LD_RTR_AHEAD_OLDER_ST                       BIT(8)
#define EHID4_DISABLE_SPEC_LS_REDIRECT                            BIT(9)
#define EHID4_DISABLE_SPEC_LDREX_PLAN_B                           BIT(10)
#define EHID4_DISABLE_DC_MVA                                      BIT(11)
#define EHID4_ENABLE_CORE_CLK_OBS_TO_SOC                          BIT(12)
#define EHID4_RCC_FORCE_ALL_LSI_L6_CLKS_ON                        BIT(13)
#define EHID4_RCC_FORCE_ALL_LSU_L3_CLKS_ON                        BIT(14)
#define EHID4_RCC_DIS_STALL_INACTIVE_LSU_CTL                      BIT(15)
#define EHID4_DISABLE_ANTI_LIVELOCK_ON_LOCK_VICTIMIZATION         BIT(16)
#define EHID4_CT0_ANTI_LIVELOCK_ON_LOCK_VICTIMIZATION(x)          ((ULONG(x)) << 17)
#define EHID4_CT0_ANTI_LIVELOCK_ON_LOCK_VICTIMIZATION_MASK        GENMASK(19, 17)
#define EHID4_CT1_ANTI_LIVELOCK_ON_LOCK_VICTIMIZATION(x)          ((ULONG(x)) << 20)
#define EHID4_CT1_ANTI_LIVELOCK_ON_LOCK_VICTIMIZATION_MASK        GENMASK(22, 20)
#define EHID4_CT2_ANTI_LIVELOCK_ON_LOCK_VICTIMIZATION(x)          ((ULONG(x)) << 23)
#define EHID4_CT2_ANTI_LIVELOCK_ON_LOCK_VICTIMIZATION_MASK        GENMASK(25, 23)
#define EHID4_CT0_ANTI_LIVELOCK_ON_OLDEST_LD_ST_REPLAY(x)         ((ULONG(x)) << 26)
#define EHID4_CT0_ANTI_LIVELOCK_ON_OLDEST_LD_ST_REPLAY_MASK       GENMASK(28, 26)
#define EHID4_CT0_ANTI_LIVELOCK_ON_LS_ISS_STALL_ON_LS_REPLAY(x)   ((ULONG(x)) << 29)
#define EHID4_CT0_ANTI_LIVELOCK_ON_LS_ISS_STALL_ON_LS_REPLAY_MASK GENMASK(31, 29)
#define EHID4_DISABLE_HW_PREF_PG_CROSS                            BIT(32)
#define EHID4_DISABLE_SPEC_LNCH_READ                              BIT(33)
#define EHID4_FORCE_NS_ORD_LD_REQ_NO_OLDER_ST                     BIT(34)
#define EHID4_RCC_FORCE_ALL_DC_DAT_L3_EN_ON                       BIT(35)
#define EHID4_UNUSED_36                                           BIT(36)
#define EHID4_FORCE_YNG_LD_FLUSH_ON_BAR_OP                        BIT(37)
#define EHID4_FORCE_YNG_LD_FLUSH_ON_LD_ACQ                        BIT(38)
#define EHID4_FORCE_NS_ORD_LD_REQ_NO_OLDER_LD                     BIT(39)
#define EHID4_STNT_COUNTER_THRESHOLD(x)                           ((ULONG(x)) << 40)
#define EHID4_STNT_COUNTER_THRESHOLD_MASK                         (3UL << 40)
#define EHID4_DISABLE_HW_PREF_ZOMBIES                             BIT(40)
#define EHID4_DISABLE_HW_PREF_ZOMBIES_LNCH_TO_CIF                 BIT(41)
#define EHID4_FORCE_BAR_LNCH_NO_OLDER_ST                          BIT(42)
#define EHID4_NON_ISA_FORCE_PROMOTE_WPT                           BIT(43)
#define EHID4_DISABLE_DC_SW_L2_OPS                                BIT(44)
#define EHID4_FORCE_NS_ORD_LD_REQ_NO_IN_PIPE_ORD_LD               BIT(45)
#define EHID4_ENABLE_LFSR_STALL_STQ_REPLAY                        BIT(48)
#define EHID4_ENABLE_EN_LFSR_STALL_LDQ_REPLAY                     BIT(49)
#define EHID4_ENABLE_LFSR_STALL_PASS2_LAUNCH                      BIT(50)
#define EHID4_ENABLE_LFSR_STALL_LSRS1_ISSUE                       BIT(51)
#define EHID4_ENABLE_LFSR_STALL_LSRS0_ISSUE                       BIT(52)
#define EHID4_ENABLE_LFSR_STALL_STQ_RTR                           BIT(53)
#define EHID4_ENABLE_LFSR_STALL_LDQ_RTR                           BIT(54)
#define EHID4_ENABLE_LFSR_STALL_SMB_DRAIN                         BIT(55)
#define EHID4_LFSR_SEED(x)                                        (3UL << 56)
#define EHID4_LFSR_SEED_MASK                                      GENMASK(62, 56)
#define EHID4_ENABLE_LFSR                                         BIT(63)

#define SYS_IMP_APL_HID5           sys_reg(3, 0, 15, 5, 0)
#define HID5_BLZ_UNK_19_18_MASK    GENMASK(19, 18)
#define HID5_BLZ_UNK18             BIT(18)
#define HID5_BLZ_UNK19             BIT(19)
#define HID5_DISABLE_FILL_2C_MERGE BIT(61)

#define SYS_IMP_APL_HID6             sys_reg(3, 0, 15, 6, 0)
#define HID6_UP_CRD_TKN_INIT_C2(x)   ((ULONG(x)) << 5)
#define HID6_UP_CRD_TKN_INIT_C2_MASK (0x1FUL << 5)

#define SYS_IMP_APL_HID7                                              sys_reg(3, 0, 15, 7, 0)
#define HID7_FORCE_NONSPEC_IF_SPEC_FLUSH_POINTER_INVALID_AND_MP_VALID BIT(16)
#define HID7_FORCE_NONSPEC_IF_STEPPING                                BIT(20)
#define HID7_FORCE_NONSPEC_TARGET_TIMER_SEL(x)                        ((ULONG(x)) << 24)
#define HID7_FORCE_NONSPEC_TARGET_TIMER_SEL_MASK                      (3UL << 24)

#define SYS_IMP_APL_HID9                sys_reg(3, 0, 15, 9, 0)
#define HID9_AVL_UNK17                  BIT(17)
#define HID9_TSO_ALLOW_DC_ZVA_WC        BIT(26)
#define HID9_TSO_SERIALIZE_VLD_MICROOPS BIT(29)
#define HID9_FIX_BUG_51667805           BIT(48)
#define HID9_FIX_BUG_55719865           BIT(55)

#define SYS_IMP_APL_EHID9               sys_reg(3, 0, 15, 9, 1)
#define EHID9_DEV_2_THROTTLE_ENABLE     BIT(5)
#define EHID9_DEV_2_THROTTLE_LIMIT_MASK GENMASK(11, 6)
#define EHID9_DEV_2_THROTTLE_LIMIT(x)   ((ULONG(x)) << 6)

#define SYS_IMP_APL_HID10               sys_reg(3, 0, 15, 10, 0)
#define SYS_IMP_APL_EHID10              sys_reg(3, 0, 15, 10, 1)
#define HID10_FORCE_WAIT_STATE_DRAIN_UC BIT(32)
#define HID10_DISABLE_ZVA_TEMPORAL_TSO  BIT(49)

#define SYS_IMP_APL_HID11            sys_reg(3, 0, 15, 11, 0)
#define HID11_ENABLE_FIX_UC_55719865 BIT(15)
#define HID11_DISABLE_LD_NT_WIDGET   BIT(59)

#define SYS_IMP_APL_HID12           sys_reg(3, 0, 15, 12, 0)

#define SYS_IMP_APL_HID13           sys_reg(3, 0, 15, 14, 0)
#define HID13_POST_OFF_CYCLES(x)    ((ULONG(x)))
#define HID13_POST_OFF_CYCLES_MASK  GENMASK(6, 0)
#define HID13_POST_ON_CYCLES(x)     ((ULONG(x)) << 7)
#define HID13_POST_ON_CYCLES_MASK   GENMASK(13, 7)
#define HID13_PRE_CYCLES(x)         ((ULONG(x)) << 14)
#define HID13_PRE_CYCLES_MASK       GENMASK(17, 14)
#define HID13_GROUP0_FF1_DELAY(x)   ((ULONG(x)) << 26)
#define HID13_GROUP0_FF1_DELAY_MASK GENMASK(29, 26)
#define HID13_GROUP0_FF2_DELAY(x)   ((ULONG(x)) << 30)
#define HID13_GROUP0_FF2_DELAY_MASK GENMASK(33, 30)
#define HID13_GROUP0_FF3_DELAY(x)   ((ULONG(x)) << 34)
#define HID13_GROUP0_FF3_DELAY_MASK GENMASK(37, 34)
#define HID13_GROUP0_FF4_DELAY(x)   ((ULONG(x)) << 38)
#define HID13_GROUP0_FF4_DELAY_MASK GENMASK(41, 38)
#define HID13_GROUP0_FF5_DELAY(x)   ((ULONG(x)) << 42)
#define HID13_GROUP0_FF5_DELAY_MASK GENMASK(45, 42)
#define HID13_GROUP0_FF6_DELAY(x)   ((ULONG(x)) << 46)
#define HID13_GROUP0_FF6_DELAY_MASK GENMASK(49, 46)
#define HID13_GROUP0_FF7_DELAY(x)   ((ULONG(x)) << 50)
#define HID13_GROUP0_FF7_DELAY_MASK GENMASK(53, 50)
#define HID13_RESET_CYCLES(x)       ((ULONG(x)) << 60)
#define HID13_RESET_CYCLES_MASK     (0xFUL << 60)

#define SYS_IMP_APL_HID14             sys_reg(3, 0, 15, 15, 0)
#define HID14_ENABLE_NEX_POWER_GATING BIT(32)

#define SYS_IMP_APL_HID16         sys_reg(3, 0, 15, 15, 2)
#define HID16_AVL_UNK12           BIT(12)
#define HID16_SPAREBIT0           BIT(56)
#define HID16_SPAREBIT3           BIT(59)
#define HID16_ENABLE_MPX_PICK_45  BIT(61)
#define HID16_ENABLE_MP_CYCLONE_7 BIT(62)

#define SYS_IMP_APL_HID18             sys_reg(3, 0, 15, 11, 2)
#define HID18_HVC_SPECULATION_DISABLE BIT(14)
#define HID18_AVL_UNK27               BIT(27)
#define HID18_AVL_UNK29               BIT(29)
#define HID18_SPAREBIT7               BIT(39)
#define HID18_SPAREBIT17              BIT(49)

#define SYS_IMP_APL_EHID18 sys_reg(3, 0, 15, 11, 3)
#define EHID18_BLZ_UNK34   BIT(34)

#define SYS_IMP_APL_EHID20                                               sys_reg(3, 0, 15, 1, 2)
#define EHID20_BLZ_DIS_HW_TRACE_SYNC_ON_UARCH_REDIR                      BIT(0)
#define EHID20_BLZ_DIS_HW_TRACE_SYNC                                     BIT(1)
#define EHID20_BLZ_DIS_AMX_FUSION_ACROSS_GRP                             BIT(2)
#define EHID20_BLZ_LSP_FORCE_SSBS_DEP                                    BIT(3)
#define EHID20_BLZ_FORCE_FP_SYNC_PRECISE_GRP_MODE                        BIT(4)
#define EHID20_BLZ_DIS_DBG_PROG_OVRD                                     BIT(5)
#define EHID20_BLZ_CONSERVATIVE_SIQ                                      BIT(6)
#define EHID20_BLZ_FORCE_DMB_ON_ANY_TSO_ENTRY_EXIT                       BIT(7)
#define EHID20_BLZ_DIS_SPEC_MDSB_INVL_ROB_FLUSH                          BIT(8)
#define EHID20_BLZ_SPARE_9                                               BIT(9)
#define EHID20_BLZ_DIS_MSR_SPEC_NON_DAIF                                 BIT(10)
#define EHID20_BLZ_DIS_MRS_SPEC_NON_DAIF                                 BIT(11)
#define EHID20_BLZ_FORCE_IEX_DISP_BIAS_MODE2                             BIT(12)
#define EHID20_BLZ_FORCE_NON_SPEC_IF_SPEC_FLUSH_PTR_INVALID_AND_MP_VALID BIT(13)
#define EHID20_BLZ_DIS_SPEC_MDSB_UNTRUSTED_INVL_ROB_FLUSH                BIT(14)
#define EHID20_BLZ_FORCE_NON_SPEC_IF_NEXT_ROB_FLUSH_DISABLED             BIT(15)
#define EHID20_BLZ_FORCE_NON_SPEC_IF_OLDEST_REDIR_VLD_AND_OLDER          BIT(16)
#define EHID20_BLZ_FORCE_NON_SPEC_IF_SPEC_FLUSH_PTR_NE_BLK_RTR_PTR       BIT(17)
#define EHID20_BLZ_FORCE_NON_SPEC_IF_STEPPING                            BIT(18)
#define EHID20_BLZ_FORCE_NON_SPEC_TIMER                                  BIT(19)
#define EHID20_BLZ_FORCE_NON_SPEC_TIMER_SEL(x)                           ((ULONG(x)) << 20)
#define EHID20_BLZ_FORCE_NON_SPEC_TIMER_SEL_MASK                         GENMASK(21, 20)
#define EHID20_BLZ_FORCE_NON_SPEC_TARGETED_TIMER_SEL(x)                  ((ULONG(x)) << 22)
#define EHID20_BLZ_FORCE_NON_SPEC_TARGETED_TIMER_SEL_MASK                GENMASK(23, 22)
#define EHID20_BLZ_DIS_LD_CBZ_TBZ_FUSION                                 BIT(24)
#define EHID20_BLZ_DIS_CMP_CSEL_FUSION                                   BIT(25)
#define EHID20_BLZ_DIS_MULT_BR_RETIRE                                    BIT(26)
#define EHID20_BLZ_DIS_INT_FV_ODD_BANKS                                  BIT(27)
#define EHID20_BLZ_DIS_NEON_FV_ODD_BANKS                                 BIT(28)
#define EHID20_BLZ_MIN_REWIND_STALL(x)                                   ((ULONG(x)) << 29)
#define EHID20_BLZ_MIN_REWIND_STALL_MASK                                 GENMASK(30, 29)
#define EHID20_BLZ_SPARE_31                                              BIT(31)
#define EHID20_BLZ_SPARE_32                                              BIT(32)
#define EHID20_BLZ_SPARE_33                                              BIT(33)
#define EHID20_BLZ_SPARE_34                                              BIT(34)
#define EHID20_BLZ_SIQ_USE_MDR_BR_SYS_RSLV                               BIT(35)
#define EHID20_BLZ_M_DSB_RESTART_IF_OLDER_REDIR                          BIT(36)
#define EHID20_BLZ_ALWAYS_INCLUSIVE_EFAR_FLUSH                           BIT(37)
#define EHID20_BLZ_UNTRUSTED_SNAPSHOT                                    BIT(38)
#define EHID20_BLZ_SPEC_MDSB_ASYNC_EXIT_STALL_FIX_DISABLE                BIT(39)
#define EHID20_BLZ_DIS_ITLB_PREF_FILL                                    BIT(53)
#define EHID20_BLZ_DIS_DMD_MERGE_PREF_FILL_FWD                           BIT(54)
#define EHID20_BLZ_DISABLE_PWR_OPT                                       BIT(55)
#define EHID20_BLZ_INC_MISS_Q_WATER_MARK(x)                              ((ULONG(x)) << 56)
#define EHID20_BLZ_INC_MISS_Q_WATER_MARK_MASK                            GENMASK(59, 56)
#define EHID20_BLZ_DISABLE_IC_MISS_PERF_OPT                              BIT(60)
#define EHID20_BLZ_DISABLE_ITLB_PREFETCH                                 BIT(61)
#define EHID20_BLZ_ENABLE_IC_TAG_CFM_PWR_OPT_ON_PAGE_BNDRY               BIT(62)
#define EHID20_BLZ_DISABLE_IC_TAG_CFM_PWR_OPT                            BIT(63)

// Older cores, off by 1?
#define EHID20_TRAP_SMC                                               BIT(8)
#define EHID20_FORCE_NONSPEC_IF_OLDEST_REDIR_VALID_AND_OLDER          BIT(15)
#define EHID20_FORCE_NONSPEC_IF_SPEC_FLUSH_POINTER_NE_BLK_RTR_POINTER BIT(16)
#define EHID20_FORCE_NONSPEC_TARGETED_TIMER_SEL(x)                    ((ULONG(x)) << 21)
#define EHID20_FORCE_NONSPEC_TARGETED_TIMER_SEL_MASK                  (3UL << 21)

#define SYS_IMP_APL_HID21                            sys_reg(3, 0, 15, 1, 3)
#define HID21_ENABLE_LDREX_FILL_REPLY                BIT(19)
#define HID21_LDQ_RTR_WAIT_FOR_OLD_ST_REL_COMPLETION BIT(33)
#define HID21_DISABLE_CDP_REPLY_PURGED_TRANSACTION   BIT(34)
#define HID21_AVL_UNK52                              BIT(52)

#define SYS_IMP_APL_HID26        sys_reg(3, 0, 15, 0, 3)
#define HID26_GROUP1_OFFSET(x)   ((ULONG(x)) << 0)
#define HID26_GROUP1_OFFSET_MASK (0xffUL << 0)
#define HID26_GROUP2_OFFSET(x)   ((ULONG(x)) << 36)
#define HID26_GROUP2_OFFSET_MASK (0xffUL << 36)

#define SYS_IMP_APL_HID27        sys_reg(3, 0, 15, 0, 4)
#define HID27_GROUP3_OFFSET(x)   ((ULONG(x)) << 8)
#define HID27_GROUP3_OFFSET_MASK (0xffUL << 8)

#define SYS_IMP_APL_PMCR0 sys_reg(3, 1, 15, 0, 0)
#define PMCR0_CNT_EN_MASK (MASK(8) | GENMASK(33, 32))
#define PMCR0_IMODE_OFF   (0 << 8)
#define PMCR0_IMODE_PMI   (1 << 8)
#define PMCR0_IMODE_AIC   (2 << 8)
#define PMCR0_IMODE_HALT  (3 << 8)
#define PMCR0_IMODE_FIQ   (4 << 8)
#define PMCR0_IMODE_MASK  (7 << 8)
#define PMCR0_IACT        (BIT(11))
#define PMCR0_PMI_SHIFT   12
#define PMCR0_CNT_MASK    (PMCR0_CNT_EN_MASK | (PMCR0_CNT_EN_MASK << PMCR0_PMI_SHIFT))

#define SYS_IMP_APL_PMCR1 sys_reg(3, 1, 15, 1, 0)
#define SYS_IMP_APL_PMCR2 sys_reg(3, 1, 15, 2, 0)
#define SYS_IMP_APL_PMCR3 sys_reg(3, 1, 15, 3, 0)
#define SYS_IMP_APL_PMCR4 sys_reg(3, 1, 15, 4, 0)

#define SYS_IMP_APL_PMESR0 sys_reg(3, 1, 15, 5, 0)
#define SYS_IMP_APL_PMESR1 sys_reg(3, 1, 15, 6, 0)

#define SYS_IMP_APL_PMSR sys_reg(3, 1, 15, 13, 0)

#define SYS_IMP_APL_PMC0 sys_reg(3, 2, 15, 0, 0)
#define SYS_IMP_APL_PMC1 sys_reg(3, 2, 15, 1, 0)
#define SYS_IMP_APL_PMC2 sys_reg(3, 2, 15, 2, 0)
#define SYS_IMP_APL_PMC3 sys_reg(3, 2, 15, 3, 0)
#define SYS_IMP_APL_PMC4 sys_reg(3, 2, 15, 4, 0)
#define SYS_IMP_APL_PMC5 sys_reg(3, 2, 15, 5, 0)
#define SYS_IMP_APL_PMC6 sys_reg(3, 2, 15, 6, 0)
#define SYS_IMP_APL_PMC7 sys_reg(3, 2, 15, 7, 0)
#define SYS_IMP_APL_PMC8 sys_reg(3, 2, 15, 9, 0)
#define SYS_IMP_APL_PMC9 sys_reg(3, 2, 15, 10, 0)

#define SYS_IMP_APL_LSU_ERR_STS   sys_reg(3, 3, 15, 0, 0)
#define SYS_IMP_APL_E_LSU_ERR_STS sys_reg(3, 3, 15, 2, 0)

#define SYS_IMP_APL_L2C_ERR_STS sys_reg(3, 3, 15, 8, 0)

#define L2C_ERR_STS_RECURSIVE_FAULT BIT(1)
#define L2C_ERR_STS_ACCESS_FAULT    BIT(7)
#define L2C_ERR_STS_ENABLE_W1C      BIT(56)

#define SYS_IMP_APL_L2C_ERR_ADR sys_reg(3, 3, 15, 9, 0)
#define SYS_IMP_APL_L2C_ERR_INF sys_reg(3, 3, 15, 10, 0)

#define SYS_IMP_APL_FED_ERR_STS   sys_reg(3, 4, 15, 0, 0)
#define SYS_IMP_APL_E_FED_ERR_STS sys_reg(3, 4, 15, 0, 2)

#define SYS_IMP_APL_SIQ_CFG_EL1 sys_reg(3, 4, 15, 10, 4)

#define SYS_IMP_APL_MMU_ERR_STS   sys_reg(3, 6, 15, 0, 0)
#define SYS_IMP_APL_E_MMU_ERR_STS sys_reg(3, 6, 15, 2, 0)

/* ACC/CYC Registers */
#define SYS_IMP_APL_ACC_CFG   sys_reg(3, 5, 15, 4, 0)
#define ACC_CFG_BP_SLEEP(x)   ((ULONG(x)) << 2)
#define ACC_CFG_BP_SLEEP_MASK (3UL << 2)

#define SYS_IMP_APL_CYC_OVRD     sys_reg(3, 5, 15, 5, 0)
#define CYC_OVRD_FIQ_MODE(x)     ((ULONG(x)) << 20)
#define CYC_OVRD_FIQ_MODE_MASK   (3UL << 20)
#define CYC_OVRD_IRQ_MODE(x)     ((ULONG(x)) << 22)
#define CYC_OVRD_IRQ_MODE_MASK   (3UL << 22)
#define CYC_OVRD_WFI_MODE(x)     ((ULONG(x)) << 24)
#define CYC_OVRD_WFI_MODE_MASK   (3UL << 24)
#define CYC_OVRD_DISABLE_WFI_RET BIT(0)

#define SYS_IMP_APL_ACC_OVRD sys_reg(3, 5, 15, 6, 0)

#define ACC_OVRD_PRE_RESET_CLK_CNTS                     (ULONG(x))
#define ACC_OVRD_PRE_RESET_CLK_CNTS_MASK                GENMASK(3, 0)
#define ACC_OVRD_FRC_PSM_COND_L3_D_ON                   BIT(4)
#define ACC_OVRD_SSB_DIVS_UPD_REQ                       (ULONG(x << 5))
#define ACC_OVRD_SSB_DIVS_UPD_REQ_MASK                  GENMASK(6, 5)
#define ACC_OVRD_SSB_DIVS_SEL                           (ULONG(x << 7))
#define ACC_OVRD_SSB_DIVS_SEL_MASK                      GENMASK(8, 7)
#define ACC_OVRD_DOM_CAN_BE_ON_IN_REF_CLK               BIT(9)
#define ACC_OVRD_HALT_LLC_BFR_RUN                       BIT(10)
#define ACC_OVRD_DIS_TB_PUSH                            BIT(11)
#define ACC_OVRD_MISR_EN                                BIT(12)
#define ACC_OVRD_PWR_DN_SRM(x)                          ((ULONG(x)) << 13)
#define ACC_OVRD_PWR_DN_SRM_MASK                        GENMASK(14, 13)
#define ACC_OVRD_DIS_L2_FLUSH_ACC_SLEEP(x)              ((ULONG(x)) << 15)
#define ACC_OVRD_DIS_L2_FLUSH_ACC_SLEEP_MASK            GENMASK(16, 15)
#define ACC_OVRD_TRAIN_DOWN_LINK(x)                     ((ULONG(x)) << 17)
#define ACC_OVRD_TRAIN_DOWN_LINK_MASK                   GENMASK(18, 17)
#define ACC_OVRD_DVFM_SAFE_VOL                          BIT(19)
#define ACC_OVRD_FRC_ZOUT_AMX_PWR_DN_TMR                BIT(20)
#define ACC_OVRD_RC_ACK_INTR_LOCK                       BIT(21)
#define ACC_OVRD_CONVERT_SIQ_TO_IRQ                     BIT(22)
#define ACC_OVRD_VOL_UPD_ACK(x)                         ((ULONG(x)) << 23)
#define ACC_OVRD_VOL_UPD_ACK_MASK                       GENMASK(24, 23)
#define ACC_OVRD_POWER_DOWN_CPM(x)                      ((ULONG(x)) << 25)
#define ACC_OVRD_POWER_DOWN_CPM_MASK                    GENMASK(26, 25)
#define ACC_OVRD_CPM_WAKE_UP(x)                         ((ULONG(x)) << 27)
#define ACC_OVRD_CPM_WAKE_UP_MASK                       GENMASK(28, 27)
#define ACC_OVRD_DISABLE_CLK_DTR                        BIT(29)
#define ACC_OVRD_DISABLE_PSW_OFF_ABORT                  BIT(30)
#define ACC_OVRD_RESET_CPM                              BIT(31)
#define ACC_OVRD_DISABLE_PIO_ON_WFI_CPU                 BIT(32)
#define ACC_OVRD_DEEP_SLEEP                             BIT(34)
#define ACC_OVRD_C2_WAKE_UP                             BIT(35)
#define ACC_OVRD_BYPASS_LLC_FLUSH_IF_EMPTY              BIT(36)
#define ACC_OVRD_DISABLE_RETENTION                      BIT(37)
#define ACC_OVRD_DISABLE_NEXT_POWER_GATE                BIT(38)
#define ACC_OVRD_AP_SKEW_N_DELAY_CTL                    BIT(39)
#define ACC_OVRD_DISABLE_C1_PPT_THROTTLE_0              BIT(40)
#define ACC_OVRD_DISABLE_C1_PPT_THROTTLE_1              BIT(41)
#define ACC_OVRD_REVERT_EMA_SEL_TO_H13                  BIT(42)
#define ACC_OVRD_DISABLE_SO_C_GLOBAL_TIME               BIT(43)
#define ACC_OVRD_QUIESCE_APSC_BEFORE_ACC_SLEEP          BIT(44)
#define ACC_OVRD_IVDM_CLK_ROOT_SEL                      BIT(45)
#define ACC_OVRD_MA_WITHOUT_SW_POLLING                  BIT(46)
#define ACC_OVRD_PMGR_UT_ADCLK_CODE                     ((ULONG(x)) << 47)
#define ACC_OVRD_PMGR_UT_ADCLK_CODE_MASK                GENMASK(50, 47)
#define ACC_OVRD_PMGR_UT_DITHER_CODE                    ((ULONG(x)) << 51)
#define ACC_OVRD_PMGR_UT_DITHER_CODE_MASK               GENMASK(54, 51)
#define ACC_OVRD_DIVS_UPD_ABRT_LIR                      BIT(55)
#define ACC_OVRD_CNVT_INC_TO_FULL_UPD                   BIT(56)
#define ACC_OVRD_DISABLE_DVTMF                          BIT(57)
#define ACC_OVRD_SKIP_BIU_DI_VS_UPD_ON_FIX_FREQ_RECLOCK BIT(58)
#define ACC_OVRD_PWR_UP_DN_CPU_AFTER_ANE_ACK            BIT(59)
#define ACC_OVRD_ANE_CLOCK_STOP_REF_COUNT               ((ULONG(x)) << 60)
#define ACC_OVRD_ANE_CLOCK_STOP_REF_COUNT_MASK          GENMASK(61, 60)
#define ACC_OVRD_SPR_OCLA_SEL                           ((ULONG(x)) << 62)
#define ACC_OVRD_SPR_OCLA_SEL_MASK                      GENMASK(63, 62)

#define SYS_IMP_APL_UPMCR0 sys_reg(3, 7, 15, 0, 4)
#define UPMCR0_IMODE_OFF   (0 << 16)
#define UPMCR0_IMODE_AIC   (2 << 16)
#define UPMCR0_IMODE_HALT  (3 << 16)
#define UPMCR0_IMODE_FIQ   (4 << 16)
#define UPMCR0_IMODE_MASK  (7 << 16)

#define SYS_IMP_APL_UPMSR sys_reg(3, 7, 15, 6, 4)
#define UPMSR_IACT        (BIT(0))

/* SPRR and GXF registers */
#define SYS_IMP_APL_SPRR_CONFIG_EL1  sys_reg(3, 6, 15, 1, 0)
#define SPRR_CONFIG_EN               BIT(0)
#define SPRR_CONFIG_LOCK_CONFIG      BIT(1)
#define SPRR_CONFIG_LOCK_PERM        BIT(4)
#define SPRR_CONFIG_LOCK_KERNEL_PERM BIT(5)

#define SYS_IMP_APL_GXF_CONFIG_EL1 sys_reg(3, 6, 15, 1, 2)
#define GXF_CONFIG_EN              BIT(0)

#define SYS_IMP_APL_GXF_STATUS_EL1 sys_reg(3, 6, 15, 8, 0)
#define GXF_STATUS_GUARDED         BIT(0)

#define SYS_IMP_APL_GXF_ABORT_EL1 sys_reg(3, 6, 15, 8, 2)
#define SYS_IMP_APL_GXF_ENTER_EL1 sys_reg(3, 6, 15, 8, 1)

#define SYS_IMP_APL_GXF_ABORT_EL12 sys_reg(3, 6, 15, 15, 3)
#define SYS_IMP_APL_GXF_ENTER_EL12 sys_reg(3, 6, 15, 15, 2)

#define SYS_IMP_APL_SPRR_PERM_EL0  sys_reg(3, 6, 15, 1, 5)
#define SYS_IMP_APL_SPRR_PERM_EL1  sys_reg(3, 6, 15, 1, 6)
#define SYS_IMP_APL_SPRR_PERM_EL02 sys_reg(3, 4, 15, 5, 2)
#define SYS_IMP_APL_SPRR_PERM_EL12 sys_reg(3, 6, 15, 15, 7)

#define SYS_IMP_APL_TPIDR_GL1 sys_reg(3, 6, 15, 10, 1)
#define SYS_IMP_APL_VBAR_GL1  sys_reg(3, 6, 15, 10, 2)
#define SYS_IMP_APL_SPSR_GL1  sys_reg(3, 6, 15, 10, 3)
#define SYS_IMP_APL_ASPSR_GL1 sys_reg(3, 6, 15, 10, 4)
#define SYS_IMP_APL_ESR_GL1   sys_reg(3, 6, 15, 10, 5)
#define SYS_IMP_APL_ELR_GL1   sys_reg(3, 6, 15, 10, 6)
#define SYS_IMP_APL_FAR_GL1   sys_reg(3, 6, 15, 10, 7)

#define SYS_IMP_APL_VBAR_GL12  sys_reg(3, 6, 15, 9, 2)
#define SYS_IMP_APL_SPSR_GL12  sys_reg(3, 6, 15, 9, 3)
#define SYS_IMP_APL_ASPSR_GL12 sys_reg(3, 6, 15, 9, 4)
#define SYS_IMP_APL_ESR_GL12   sys_reg(3, 6, 15, 9, 5)
#define SYS_IMP_APL_ELR_GL12   sys_reg(3, 6, 15, 9, 6)
#define SYS_IMP_APL_SP_GL12    sys_reg(3, 6, 15, 10, 0)

#define SYS_IMP_APL_AFSR1_GL1 sys_reg(3, 6, 15, 0, 1)

/* PAuth registers */
#define SYS_IMP_APL_APVMKEYLO_EL2 sys_reg(3, 6, 15, 14, 4)
#define SYS_IMP_APL_APVMKEYHI_EL2 sys_reg(3, 6, 15, 14, 5)
#define SYS_IMP_APL_APSTS_EL12    sys_reg(3, 6, 15, 14, 7)

#define SYS_IMP_APL_APCTL_EL1  sys_reg(3, 4, 15, 0, 4)
#define SYS_IMP_APL_APCTL_EL2  sys_reg(3, 6, 15, 12, 2)
#define SYS_IMP_APL_APCTL_EL12 sys_reg(3, 6, 15, 15, 0)

/* VM registers */
#define SYS_IMP_APL_VM_TMR_FIQ_ENA_EL2 sys_reg(3, 5, 15, 1, 3)
#define VM_TMR_FIQ_ENA_ENA_V           BIT(0)
#define VM_TMR_FIQ_ENA_ENA_P           BIT(1)

/* IPI registers */
#define SYS_IMP_APL_IPI_RR_LOCAL_EL1  sys_reg(3, 5, 15, 0, 0)
#define SYS_IMP_APL_IPI_RR_GLOBAL_EL1 sys_reg(3, 5, 15, 0, 1)

#define SYS_IMP_APL_IPI_SR_EL1 sys_reg(3, 5, 15, 1, 1)
#define IPI_SR_PENDING         BIT(0)

#define SYS_IMP_APL_IPI_CR_EL1 sys_reg(3, 5, 15, 3, 1)
