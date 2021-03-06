/*
 * modules/gpu/sgx544/linux/kernel_mode/eurasia_km/services4/srvkm/hwdefs/sgxmpdefs.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*************************************************************************/ /*!
@Title          Hardware defs for SGXMP.
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@License        Dual MIT/GPLv2

The contents of this file are subject to the MIT license as set out below.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public License Version 2 ("GPL") in which case the provisions
of GPL are applicable instead of those above.

If you wish to allow use of your version of this file only under the terms of
GPL, and not to allow others to use your version of this file under the terms
of the MIT license, indicate your decision by deleting the provisions above
and replace them with the notice and other provisions required by GPL as set
out in the file called "GPL-COPYING" included in this distribution. If you do
not delete the provisions above, a recipient may use your version of this file
under the terms of either the MIT license or GPL.

This License is also included in this distribution in the file called
"MIT-COPYING".

EXCEPT AS OTHERWISE STATED IN A NEGOTIATED AGREEMENT: (A) THE SOFTWARE IS
PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT; AND (B) IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ /**************************************************************************/

#ifndef _SGXMPDEFS_KM_H_
#define _SGXMPDEFS_KM_H_

/* Register EUR_CR_MASTER_BIF_CTRL */
#define EUR_CR_MASTER_BIF_CTRL              0x4C00
#define EUR_CR_MASTER_BIF_CTRL_NOREORDER_MASK 0x00000001U
#define EUR_CR_MASTER_BIF_CTRL_NOREORDER_SHIFT 0
#define EUR_CR_MASTER_BIF_CTRL_NOREORDER_SIGNED 0
#define EUR_CR_MASTER_BIF_CTRL_PAUSE_MASK   0x00000002U
#define EUR_CR_MASTER_BIF_CTRL_PAUSE_SHIFT  1
#define EUR_CR_MASTER_BIF_CTRL_PAUSE_SIGNED 0
#define EUR_CR_MASTER_BIF_CTRL_CLEAR_FAULT_MASK 0x00000010U
#define EUR_CR_MASTER_BIF_CTRL_CLEAR_FAULT_SHIFT 4
#define EUR_CR_MASTER_BIF_CTRL_CLEAR_FAULT_SIGNED 0
#define EUR_CR_MASTER_BIF_CTRL_MMU_BYPASS_PTLA_MASK 0x00010000U
#define EUR_CR_MASTER_BIF_CTRL_MMU_BYPASS_PTLA_SHIFT 16
#define EUR_CR_MASTER_BIF_CTRL_MMU_BYPASS_PTLA_SIGNED 0
#define EUR_CR_MASTER_BIF_CTRL_MMU_BYPASS_MASTER_VDM_MASK 0x00020000U
#define EUR_CR_MASTER_BIF_CTRL_MMU_BYPASS_MASTER_VDM_SHIFT 17
#define EUR_CR_MASTER_BIF_CTRL_MMU_BYPASS_MASTER_VDM_SIGNED 0
#define EUR_CR_MASTER_BIF_CTRL_MMU_BYPASS_MASTER_IPF_MASK 0x00040000U
#define EUR_CR_MASTER_BIF_CTRL_MMU_BYPASS_MASTER_IPF_SHIFT 18
#define EUR_CR_MASTER_BIF_CTRL_MMU_BYPASS_MASTER_IPF_SIGNED 0
#define EUR_CR_MASTER_BIF_CTRL_MMU_BYPASS_MASTER_DPM_MASK 0x00080000U
#define EUR_CR_MASTER_BIF_CTRL_MMU_BYPASS_MASTER_DPM_SHIFT 19
#define EUR_CR_MASTER_BIF_CTRL_MMU_BYPASS_MASTER_DPM_SIGNED 0
/* Register EUR_CR_MASTER_BIF_INT_STAT */
#define EUR_CR_MASTER_BIF_INT_STAT          0x4C04
#define EUR_CR_MASTER_BIF_INT_STAT_FAULT_REQ_MASK 0x00003FFFU
#define EUR_CR_MASTER_BIF_INT_STAT_FAULT_REQ_SHIFT 0
#define EUR_CR_MASTER_BIF_INT_STAT_FAULT_REQ_SIGNED 0
#define EUR_CR_MASTER_BIF_INT_STAT_FAULT_TYPE_MASK 0x00070000U
#define EUR_CR_MASTER_BIF_INT_STAT_FAULT_TYPE_SHIFT 16
#define EUR_CR_MASTER_BIF_INT_STAT_FAULT_TYPE_SIGNED 0
#define EUR_CR_MASTER_BIF_INT_STAT_FLUSH_COMPLETE_MASK 0x00080000U
#define EUR_CR_MASTER_BIF_INT_STAT_FLUSH_COMPLETE_SHIFT 19
#define EUR_CR_MASTER_BIF_INT_STAT_FLUSH_COMPLETE_SIGNED 0
/* Register EUR_CR_MASTER_BIF_FAULT */
#define EUR_CR_MASTER_BIF_FAULT             0x4C08
#define EUR_CR_MASTER_BIF_FAULT_CID_MASK    0x0000000FU
#define EUR_CR_MASTER_BIF_FAULT_CID_SHIFT   0
#define EUR_CR_MASTER_BIF_FAULT_CID_SIGNED  0
#define EUR_CR_MASTER_BIF_FAULT_SB_MASK     0x000001F0U
#define EUR_CR_MASTER_BIF_FAULT_SB_SHIFT    4
#define EUR_CR_MASTER_BIF_FAULT_SB_SIGNED   0
#define EUR_CR_MASTER_BIF_FAULT_ADDR_MASK   0xFFFFF000U
#define EUR_CR_MASTER_BIF_FAULT_ADDR_SHIFT  12
#define EUR_CR_MASTER_BIF_FAULT_ADDR_SIGNED 0
/* Register EUR_CR_MASTER_BIF_CTRL_INVAL */
#define EUR_CR_MASTER_BIF_CTRL_INVAL        0x4C34
#define EUR_CR_MASTER_BIF_CTRL_INVAL_PTE_MASK 0x00000004U
#define EUR_CR_MASTER_BIF_CTRL_INVAL_PTE_SHIFT 2
#define EUR_CR_MASTER_BIF_CTRL_INVAL_PTE_SIGNED 0
#define EUR_CR_MASTER_BIF_CTRL_INVAL_ALL_MASK 0x00000008U
#define EUR_CR_MASTER_BIF_CTRL_INVAL_ALL_SHIFT 3
#define EUR_CR_MASTER_BIF_CTRL_INVAL_ALL_SIGNED 0
/* Register EUR_CR_MASTER_BIF_MMU_CTRL */
#define EUR_CR_MASTER_BIF_MMU_CTRL          0x4CD0
#define EUR_CR_MASTER_BIF_MMU_CTRL_PREFETCHING_ON_MASK 0x00000001U
#define EUR_CR_MASTER_BIF_MMU_CTRL_PREFETCHING_ON_SHIFT 0
#define EUR_CR_MASTER_BIF_MMU_CTRL_PREFETCHING_ON_SIGNED 0
#define EUR_CR_MASTER_BIF_MMU_CTRL_ADDR_HASH_MODE_MASK 0x00000006U
#define EUR_CR_MASTER_BIF_MMU_CTRL_ADDR_HASH_MODE_SHIFT 1
#define EUR_CR_MASTER_BIF_MMU_CTRL_ADDR_HASH_MODE_SIGNED 0
#define EUR_CR_MASTER_BIF_MMU_CTRL_ENABLE_DC_TLB_MASK 0x00000010U
#define EUR_CR_MASTER_BIF_MMU_CTRL_ENABLE_DC_TLB_SHIFT 4
#define EUR_CR_MASTER_BIF_MMU_CTRL_ENABLE_DC_TLB_SIGNED 0
/* Register EUR_CR_MASTER_SLC_CTRL */
#define EUR_CR_MASTER_SLC_CTRL              0x4D00
#define EUR_CR_MASTER_SLC_CTRL_DISABLE_REORDERING_MASK 0x00800000U
#define EUR_CR_MASTER_SLC_CTRL_DISABLE_REORDERING_SHIFT 23
#define EUR_CR_MASTER_SLC_CTRL_DISABLE_REORDERING_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_DISABLE_BURST_EXP_MASK 0x00400000U
#define EUR_CR_MASTER_SLC_CTRL_DISABLE_BURST_EXP_SHIFT 22
#define EUR_CR_MASTER_SLC_CTRL_DISABLE_BURST_EXP_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_REQ3_MASK 0x00200000U
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_REQ3_SHIFT 21
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_REQ3_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_REQ2_MASK 0x00100000U
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_REQ2_SHIFT 20
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_REQ2_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_REQ1_MASK 0x00080000U
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_REQ1_SHIFT 19
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_REQ1_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_REQ0_MASK 0x00040000U
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_REQ0_SHIFT 18
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_REQ0_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_DM_REF_SET_ALL_MASK 0x00010000U
#define EUR_CR_MASTER_SLC_CTRL_DM_REF_SET_ALL_SHIFT 16
#define EUR_CR_MASTER_SLC_CTRL_DM_REF_SET_ALL_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_ARB_PAGE_SIZE_MASK 0x0000F000U
#define EUR_CR_MASTER_SLC_CTRL_ARB_PAGE_SIZE_SHIFT 12
#define EUR_CR_MASTER_SLC_CTRL_ARB_PAGE_SIZE_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_ADDR_DECODE_MODE_MASK 0x00000E00U
#define EUR_CR_MASTER_SLC_CTRL_ADDR_DECODE_MODE_SHIFT 9
#define EUR_CR_MASTER_SLC_CTRL_ADDR_DECODE_MODE_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_PAUSE_MASK   0x00000100U
#define EUR_CR_MASTER_SLC_CTRL_PAUSE_SHIFT  8
#define EUR_CR_MASTER_SLC_CTRL_PAUSE_SIGNED 0
/* Register EUR_CR_MASTER_SLC_CTRL_BYPASS */
#define EUR_CR_MASTER_SLC_CTRL_BYPASS       0x4D04
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_BYP_CC_N_MASK 0x08000000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_BYP_CC_N_SHIFT 27
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_BYP_CC_N_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_BYP_CC_MASK 0x04000000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_BYP_CC_SHIFT 26
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_BYP_CC_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE4_MASK 0x02000000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE4_SHIFT 25
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE4_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE3_MASK 0x01000000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE3_SHIFT 24
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE3_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE2_MASK 0x00800000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE2_SHIFT 23
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE2_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE1_MASK 0x00400000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE1_SHIFT 22
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE1_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE0_MASK 0x00200000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE0_SHIFT 21
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_CORE0_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_PTLA_MASK 0x00100000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_PTLA_SHIFT 20
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_PTLA_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_ISP2_RCIF_MASK 0x00080000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_ISP2_RCIF_SHIFT 19
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_ISP2_RCIF_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_ZLS_MASK 0x00040000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_ZLS_SHIFT 18
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_ZLS_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_PBE_MASK 0x00020000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_PBE_SHIFT 17
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_PBE_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_VDM_MASK 0x00010000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_VDM_SHIFT 16
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_VDM_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_IPF_MASK 0x00008000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_IPF_SHIFT 15
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_IPF_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_PDS_MASK 0x00004000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_PDS_SHIFT 14
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_PDS_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USEC_MASK 0x00002000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USEC_SHIFT 13
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USEC_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USE3_MASK 0x00001000U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USE3_SHIFT 12
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USE3_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USE2_MASK 0x00000800U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USE2_SHIFT 11
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USE2_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USE1_MASK 0x00000400U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USE1_SHIFT 10
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USE1_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USE0_MASK 0x00000200U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USE0_SHIFT 9
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_USE0_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_IPF_OBJ_MASK 0x00000100U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_IPF_OBJ_SHIFT 8
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_IPF_OBJ_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_TPF_MASK 0x00000080U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_TPF_SHIFT 7
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_TPF_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_TA_MASK 0x00000040U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_TA_SHIFT 6
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_TA_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_CACHE_MASK 0x00000020U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_CACHE_SHIFT 5
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_CACHE_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_MMU_MASK 0x00000010U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_MMU_SHIFT 4
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_REQ_MMU_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_DM_EVENT_MASK 0x00000008U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_DM_EVENT_SHIFT 3
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_DM_EVENT_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_DM_PIXEL_MASK 0x00000004U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_DM_PIXEL_SHIFT 2
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_DM_PIXEL_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_DM_VERTEX_MASK 0x00000002U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_DM_VERTEX_SHIFT 1
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_DM_VERTEX_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_ALL_MASK 0x00000001U
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_ALL_SHIFT 0
#define EUR_CR_MASTER_SLC_CTRL_BYPASS_ALL_SIGNED 0
/* Register EUR_CR_MASTER_SLC_CTRL_USSE_INVAL */
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL   0x4D08
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_ADDR_MASK 0xFFFFFFFFU
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_ADDR_SHIFT 0
#define EUR_CR_MASTER_SLC_CTRL_USSE_INVAL_ADDR_SIGNED 0
/* Register EUR_CR_MASTER_SLC_CTRL_INVAL */
#define EUR_CR_MASTER_SLC_CTRL_INVAL        0x4D28
#define EUR_CR_MASTER_SLC_CTRL_INVAL_DM_EVENT_MASK 0x00000008U
#define EUR_CR_MASTER_SLC_CTRL_INVAL_DM_EVENT_SHIFT 3
#define EUR_CR_MASTER_SLC_CTRL_INVAL_DM_EVENT_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_INVAL_DM_PIXEL_MASK 0x00000004U
#define EUR_CR_MASTER_SLC_CTRL_INVAL_DM_PIXEL_SHIFT 2
#define EUR_CR_MASTER_SLC_CTRL_INVAL_DM_PIXEL_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_INVAL_DM_VERTEX_MASK 0x00000002U
#define EUR_CR_MASTER_SLC_CTRL_INVAL_DM_VERTEX_SHIFT 1
#define EUR_CR_MASTER_SLC_CTRL_INVAL_DM_VERTEX_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_INVAL_ALL_MASK 0x00000001U
#define EUR_CR_MASTER_SLC_CTRL_INVAL_ALL_SHIFT 0
#define EUR_CR_MASTER_SLC_CTRL_INVAL_ALL_SIGNED 0
/* Register EUR_CR_MASTER_SLC_CTRL_FLUSH */
#define EUR_CR_MASTER_SLC_CTRL_FLUSH        0x4D2C
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_DM_EVENT_MASK 0x00000080U
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_DM_EVENT_SHIFT 7
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_DM_EVENT_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_DM_PIXEL_MASK 0x00000040U
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_DM_PIXEL_SHIFT 6
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_DM_PIXEL_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_DM_VERTEX_MASK 0x00000020U
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_DM_VERTEX_SHIFT 5
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_DM_VERTEX_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_ALL_MASK 0x00000010U
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_ALL_SHIFT 4
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_ALL_SIGNED 0
/* Register EUR_CR_MASTER_SLC_CTRL_FLUSH_INV */
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_INV    0x4D34
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_INV_DM_EVENT_MASK 0x00000080U
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_INV_DM_EVENT_SHIFT 7
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_INV_DM_EVENT_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_INV_DM_PIXEL_MASK 0x00000040U
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_INV_DM_PIXEL_SHIFT 6
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_INV_DM_PIXEL_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_INV_DM_VERTEX_MASK 0x00000020U
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_INV_DM_VERTEX_SHIFT 5
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_INV_DM_VERTEX_SIGNED 0
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_INV_ALL_MASK 0x00000010U
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_INV_ALL_SHIFT 4
#define EUR_CR_MASTER_SLC_CTRL_FLUSH_INV_ALL_SIGNED 0
/* Register EUR_CR_MASTER_BREAKPOINT_READ */
#define EUR_CR_MASTER_BREAKPOINT_READ       0x4F18
#define EUR_CR_MASTER_BREAKPOINT_READ_ADDRESS_MASK 0xFFFFFFF0U
#define EUR_CR_MASTER_BREAKPOINT_READ_ADDRESS_SHIFT 4
#define EUR_CR_MASTER_BREAKPOINT_READ_ADDRESS_SIGNED 0
/* Register EUR_CR_MASTER_BREAKPOINT_TRAP */
#define EUR_CR_MASTER_BREAKPOINT_TRAP       0x4F1C
#define EUR_CR_MASTER_BREAKPOINT_TRAP_CONTINUE_MASK 0x00000002U
#define EUR_CR_MASTER_BREAKPOINT_TRAP_CONTINUE_SHIFT 1
#define EUR_CR_MASTER_BREAKPOINT_TRAP_CONTINUE_SIGNED 0
#define EUR_CR_MASTER_BREAKPOINT_TRAP_WRNOTIFY_MASK 0x00000001U
#define EUR_CR_MASTER_BREAKPOINT_TRAP_WRNOTIFY_SHIFT 0
#define EUR_CR_MASTER_BREAKPOINT_TRAP_WRNOTIFY_SIGNED 0
/* Register EUR_CR_MASTER_BREAKPOINT */
#define EUR_CR_MASTER_BREAKPOINT            0x4F20
#define EUR_CR_MASTER_BREAKPOINT_ID_MASK    0x00000030U
#define EUR_CR_MASTER_BREAKPOINT_ID_SHIFT   4
#define EUR_CR_MASTER_BREAKPOINT_ID_SIGNED  0
#define EUR_CR_MASTER_BREAKPOINT_UNTRAPPED_MASK 0x00000008U
#define EUR_CR_MASTER_BREAKPOINT_UNTRAPPED_SHIFT 3
#define EUR_CR_MASTER_BREAKPOINT_UNTRAPPED_SIGNED 0
#define EUR_CR_MASTER_BREAKPOINT_TRAPPED_MASK 0x00000004U
#define EUR_CR_MASTER_BREAKPOINT_TRAPPED_SHIFT 2
#define EUR_CR_MASTER_BREAKPOINT_TRAPPED_SIGNED 0
/* Register EUR_CR_MASTER_BREAKPOINT_TRAP_INFO0 */
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO0 0x4F24
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO0_ADDRESS_MASK 0xFFFFFFF0U
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO0_ADDRESS_SHIFT 4
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO0_ADDRESS_SIGNED 0
/* Register EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1 */
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1 0x4F28
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_SIZE_MASK 0x00007C00U
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_SIZE_SHIFT 10
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_SIZE_SIGNED 0
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_NUMBER_MASK 0x00000300U
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_NUMBER_SHIFT 8
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_NUMBER_SIGNED 0
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_TAG_MASK 0x000000F8U
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_TAG_SHIFT 3
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_TAG_SIGNED 0
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_DATA_MASTER_MASK 0x00000006U
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_DATA_MASTER_SHIFT 1
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_DATA_MASTER_SIGNED 0
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_RNW_MASK 0x00000001U
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_RNW_SHIFT 0
#define EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1_RNW_SIGNED 0
/* Register EUR_CR_MASTER_CORE */
#define EUR_CR_MASTER_CORE                  0x4000
#define EUR_CR_MASTER_CORE_ENABLE_MASK      0x00000003U
#define EUR_CR_MASTER_CORE_ENABLE_SHIFT     0
#define EUR_CR_MASTER_CORE_ENABLE_SIGNED    0
/* Register EUR_CR_MASTER_CORE_ID */
#define EUR_CR_MASTER_CORE_ID               0x4010
#define EUR_CR_MASTER_CORE_ID_CONFIG_MULTI_MASK 0x00000001U
#define EUR_CR_MASTER_CORE_ID_CONFIG_MULTI_SHIFT 0
#define EUR_CR_MASTER_CORE_ID_CONFIG_MULTI_SIGNED 0
#define EUR_CR_MASTER_CORE_ID_CONFIG_BASE_MASK 0x00000002U
#define EUR_CR_MASTER_CORE_ID_CONFIG_BASE_SHIFT 1
#define EUR_CR_MASTER_CORE_ID_CONFIG_BASE_SIGNED 0
#define EUR_CR_MASTER_CORE_ID_CONFIG_MASK   0x000000FCU
#define EUR_CR_MASTER_CORE_ID_CONFIG_SHIFT  2
#define EUR_CR_MASTER_CORE_ID_CONFIG_SIGNED 0
#define EUR_CR_MASTER_CORE_ID_CONFIG_CORES_MASK 0x00000F00U
#define EUR_CR_MASTER_CORE_ID_CONFIG_CORES_SHIFT 8
#define EUR_CR_MASTER_CORE_ID_CONFIG_CORES_SIGNED 0
#define EUR_CR_MASTER_CORE_ID_CONFIG_SLC_MASK 0x0000F000U
#define EUR_CR_MASTER_CORE_ID_CONFIG_SLC_SHIFT 12
#define EUR_CR_MASTER_CORE_ID_CONFIG_SLC_SIGNED 0
#define EUR_CR_MASTER_CORE_ID_ID_MASK       0xFFFF0000U
#define EUR_CR_MASTER_CORE_ID_ID_SHIFT      16
#define EUR_CR_MASTER_CORE_ID_ID_SIGNED     0
/* Register EUR_CR_MASTER_CORE_REVISION */
#define EUR_CR_MASTER_CORE_REVISION         0x4014
#define EUR_CR_MASTER_CORE_REVISION_MAINTENANCE_MASK 0x000000FFU
#define EUR_CR_MASTER_CORE_REVISION_MAINTENANCE_SHIFT 0
#define EUR_CR_MASTER_CORE_REVISION_MAINTENANCE_SIGNED 0
#define EUR_CR_MASTER_CORE_REVISION_MINOR_MASK 0x0000FF00U
#define EUR_CR_MASTER_CORE_REVISION_MINOR_SHIFT 8
#define EUR_CR_MASTER_CORE_REVISION_MINOR_SIGNED 0
#define EUR_CR_MASTER_CORE_REVISION_MAJOR_MASK 0x00FF0000U
#define EUR_CR_MASTER_CORE_REVISION_MAJOR_SHIFT 16
#define EUR_CR_MASTER_CORE_REVISION_MAJOR_SIGNED 0
#define EUR_CR_MASTER_CORE_REVISION_DESIGNER_MASK 0xFF000000U
#define EUR_CR_MASTER_CORE_REVISION_DESIGNER_SHIFT 24
#define EUR_CR_MASTER_CORE_REVISION_DESIGNER_SIGNED 0
/* Register EUR_CR_MASTER_SOFT_RESET */
#define EUR_CR_MASTER_SOFT_RESET            0x4080
#define EUR_CR_MASTER_SOFT_RESET_CORE_RESET_MASK(i) (0x00000001U << (0 + ((i) * 1)))
#define EUR_CR_MASTER_SOFT_RESET_CORE_RESET_SHIFT(i) (0 + ((i) * 1))
#define EUR_CR_MASTER_SOFT_RESET_CORE_RESET_REGNUM(i) 0x4080
#define EUR_CR_MASTER_SOFT_RESET_IPF_RESET_MASK 0x00000010U
#define EUR_CR_MASTER_SOFT_RESET_IPF_RESET_SHIFT 4
#define EUR_CR_MASTER_SOFT_RESET_IPF_RESET_SIGNED 0
#define EUR_CR_MASTER_SOFT_RESET_DPM_RESET_MASK 0x00000020U
#define EUR_CR_MASTER_SOFT_RESET_DPM_RESET_SHIFT 5
#define EUR_CR_MASTER_SOFT_RESET_DPM_RESET_SIGNED 0
#define EUR_CR_MASTER_SOFT_RESET_VDM_RESET_MASK 0x00000040U
#define EUR_CR_MASTER_SOFT_RESET_VDM_RESET_SHIFT 6
#define EUR_CR_MASTER_SOFT_RESET_VDM_RESET_SIGNED 0
#define EUR_CR_MASTER_SOFT_RESET_SLC_RESET_MASK 0x00000080U
#define EUR_CR_MASTER_SOFT_RESET_SLC_RESET_SHIFT 7
#define EUR_CR_MASTER_SOFT_RESET_SLC_RESET_SIGNED 0
#define EUR_CR_MASTER_SOFT_RESET_BIF_RESET_MASK 0x00000100U
#define EUR_CR_MASTER_SOFT_RESET_BIF_RESET_SHIFT 8
#define EUR_CR_MASTER_SOFT_RESET_BIF_RESET_SIGNED 0
#define EUR_CR_MASTER_SOFT_RESET_MCI_RESET_MASK 0x00000200U
#define EUR_CR_MASTER_SOFT_RESET_MCI_RESET_SHIFT 9
#define EUR_CR_MASTER_SOFT_RESET_MCI_RESET_SIGNED 0
#define EUR_CR_MASTER_SOFT_RESET_PTLA_RESET_MASK 0x00000400U
#define EUR_CR_MASTER_SOFT_RESET_PTLA_RESET_SHIFT 10
#define EUR_CR_MASTER_SOFT_RESET_PTLA_RESET_SIGNED 0

#endif /* _SGXMPDEFS_KM_H_ */

