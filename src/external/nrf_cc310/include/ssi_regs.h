//-----------------------------------------------------------------------------------
//    The confidential and proprietary information contained in this file may 
//    only be used by a person authorised under and to the extent permitted
//    by a subsisting licensing agreement from ARM Limited or its affiliates 
//    or between you and a party authorised by ARM
//
//           (C) COPYRIGHT [2016] ARM Limited or its affiliates
//               ALL RIGHT RESERVED
//
//    This entire notice must be reproduced on all copies of this file
//    and copies of this files may only be made by a person if such person is
//    permitted to do so under the terms of a subsisting license agreement
//    from ARM Limited or its affiliates or between you and a party authorised by ARM
//-----------------------------------------------------------------------------------


/*!
 * @file
 * @brief Macro definitions for accessing ARM TrustZone CryptoCell register space.
 */

#ifndef _SSI_REGS_H_
#define _SSI_REGS_H_

#include "ssi_bitops.h"

/* Register Offset macro */
#define SASI_REG_OFFSET(unit_name, reg_name)               \
	(DX_BASE_ ## unit_name + DX_ ## reg_name ## _REG_OFFSET)

#define SASI_REG_BIT_SHIFT(reg_name, field_name)               \
	(DX_ ## reg_name ## _ ## field_name ## _BIT_SHIFT)

/* Register Offset macros (from registers base address in host) */
#if defined(DX_CC_REE) || defined(DX_CC_TEE) || defined(DX_CONFIG_IOT_SUPPORTED)

#include "dx_reg_base_host.h"

/* Indexed GPR offset macros - note the (not original) preprocessor tricks...*/
/* (Using the macro without the "_" prefix is allowed with another macro      *
 *  as the gpr_idx) */
#define _SEP_HOST_GPR_REG_OFFSET(gpr_idx) \
	SASI_REG_OFFSET(HOST_RGF, HOST_SEP_HOST_GPR ## gpr_idx)
#define SEP_HOST_GPR_REG_OFFSET(gpr_idx) _SEP_HOST_GPR_REG_OFFSET(gpr_idx)
#define _HOST_SEP_GPR_REG_OFFSET(gpr_idx) \
	SASI_REG_OFFSET(HOST_RGF, HOST_HOST_SEP_GPR ## gpr_idx)
#define HOST_SEP_GPR_REG_OFFSET(gpr_idx) _HOST_SEP_GPR_REG_OFFSET(gpr_idx)

/* GPR IRQ bit shift/mask by GPR index */
#define _SEP_HOST_GPR_IRQ_SHIFT(gpr_idx) \
	DX_HOST_IRR_SEP_HOST_GPR ## gpr_idx ## _INT_BIT_SHIFT
#define SEP_HOST_GPR_IRQ_SHIFT(gpr_idx) \
	_SEP_HOST_GPR_IRQ_SHIFT(gpr_idx)
#define SEP_HOST_GPR_IRQ_MASK(gpr_idx) \
	(1 << SEP_HOST_GPR_IRQ_CAUSE_SHIFT(gpr_idx))

/* Read-Modify-Write a field of a register */
#define MODIFY_REGISTER_FLD(unitName, regName, fldName, fldVal)         \
do {								            \
	uint32_t regVal;						    \
	regVal = READ_REGISTER(SASI_REG_ADDR(unitName, regName));       \
	SASI_REG_FLD_SET(unitName, regName, fldName, regVal, fldVal); \
	WRITE_REGISTER(SASI_REG_ADDR(unitName, regName), regVal);       \
} while (0)

#elif defined(DX_CC_SEP)

#include "dx_reg_base_sep.h"

/* We only provide this macro for SEP code because other platforms require usage
   of (mapping base + offset), i.e., use of SASI_REG_OFFSET */
/* PLEASE AVOID USING THIS MACRO FOR NEW SEP CODE. USE THE OFFSET MACROS. */
#define SASI_REG_ADDR(unit_name, reg_name)            \
	 (DX_BASE_CC_PERIF + DX_BASE_ ## unit_name + \
	  DX_ ## reg_name ## _REG_OFFSET)

/* Indexed GPR address macros - note the (not original) preprocessor tricks...*/
/* (Using the macro without the "_" prefix is allowed with another macro      *
 *  as the gpr_idx) */
#define _SEP_HOST_GPR_REG_ADDR(gpr_idx) \
	SASI_REG_ADDR(SEP_RGF, SEP_SEP_HOST_GPR ## gpr_idx)
#define SEP_HOST_GPR_REG_ADDR(gpr_idx) _SEP_HOST_GPR_REG_ADDR(gpr_idx)
#define _HOST_SEP_GPR_REG_ADDR(gpr_idx) \
	SASI_REG_ADDR(SEP_RGF, SEP_HOST_SEP_GPR ## gpr_idx)
#define HOST_SEP_GPR_REG_ADDR(gpr_idx) _HOST_SEP_GPR_REG_ADDR(gpr_idx)

#else
#error Execution domain is not DX_CC_SEP/DX_CC_REE/DX_CC_TEE/DX_CONFIG_IOT_SUPPORTED
#endif

/* Registers address macros for ENV registers (development FPGA only) */
#ifdef DX_BASE_ENV_REGS

#ifndef DX_CC_SEP /* Irrelevant for SeP code */
/* This offset should be added to mapping address of DX_BASE_ENV_REGS */
#define SASI_ENV_REG_OFFSET(reg_name) (DX_ENV_ ## reg_name ## _REG_OFFSET)
#endif

#endif /*DX_BASE_ENV_REGS*/

/* Bit fields access */
#define SASI_REG_FLD_GET(unit_name, reg_name, fld_name, reg_val)	      \
	(DX_ ## reg_name ## _ ## fld_name ## _BIT_SIZE == 0x20 ?	      \
	reg_val /*!< \internal Optimization for 32b fields */ :			      \
	BITFIELD_GET(reg_val, DX_ ## reg_name ## _ ## fld_name ## _BIT_SHIFT, \
		     DX_ ## reg_name ## _ ## fld_name ## _BIT_SIZE))

/* Bit fields access */
#define SASI2_REG_FLD_GET(unit_name, reg_name, fld_name, reg_val)	      \
	(SASI_ ## reg_name ## _ ## fld_name ## _BIT_SIZE == 0x20 ?	      \
	reg_val /*!< \internal Optimization for 32b fields */ :			      \
	BITFIELD_GET(reg_val, SASI_ ## reg_name ## _ ## fld_name ## _BIT_SHIFT, \
		     SASI_ ## reg_name ## _ ## fld_name ## _BIT_SIZE))

#define SASI_REG_FLD_SET(                                               \
	unit_name, reg_name, fld_name, reg_shadow_var, new_fld_val)      \
do {                                                                     \
	if (DX_ ## reg_name ## _ ## fld_name ## _BIT_SIZE == 0x20)       \
		reg_shadow_var = new_fld_val; /*!< \internal Optimization for 32b fields */\
	else                                                             \
		BITFIELD_SET(reg_shadow_var,                             \
			DX_ ## reg_name ## _ ## fld_name ## _BIT_SHIFT,  \
			DX_ ## reg_name ## _ ## fld_name ## _BIT_SIZE,   \
			new_fld_val);                                    \
} while (0)

#define SASI2_REG_FLD_SET(                                               \
	unit_name, reg_name, fld_name, reg_shadow_var, new_fld_val)      \
do {                                                                     \
	if (SASI_ ## reg_name ## _ ## fld_name ## _BIT_SIZE == 0x20)       \
		reg_shadow_var = new_fld_val; /*!< \internal Optimization for 32b fields */\
	else                                                             \
		BITFIELD_SET(reg_shadow_var,                             \
			SASI_ ## reg_name ## _ ## fld_name ## _BIT_SHIFT,  \
			SASI_ ## reg_name ## _ ## fld_name ## _BIT_SIZE,   \
			new_fld_val);                                    \
} while (0)

/* Usage example:
   uint32_t reg_shadow = READ_REGISTER(SASI_REG_ADDR(CRY_KERNEL,AES_CONTROL));
   SASI_REG_FLD_SET(CRY_KERNEL,AES_CONTROL,NK_KEY0,reg_shadow, 3);
   SASI_REG_FLD_SET(CRY_KERNEL,AES_CONTROL,NK_KEY1,reg_shadow, 1);
   WRITE_REGISTER(SASI_REG_ADDR(CRY_KERNEL,AES_CONTROL), reg_shadow);
 */

#endif /*_SSI_REGS_H_*/
