/*****************************************************************************
 * reg_view.h
 *
 * Copyright (C) 2017 onejacky <wanshijie@126.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 ****************************************************************************/

#ifndef REG_VIEW_H_INCLUDE
#define REG_VIEW_H_INCLUDE
/*****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "sys_list.h"

/*****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define REG_BF_MAX_SIZE     32
#define REG_BF_OPT_MAX_SIZE 32

/*****************************************************************************
 * Public Types
 ****************************************************************************/

enum reg_bf_attr {
    REG_BF_ATTR_UNK = 0, /* reserved        */
    REG_BF_ATTR_RO  = 1, /* read only       */
    REG_BF_ATTR_RW  = 2, /* read/write      */
    REG_BF_ATTR_RC  = 3, /* read one clear  */
    REG_BF_ATTR_WC  = 4, /* write one clear */
};

struct reg_bf_option {
    bool      reserved;
    uint32_t  val;
    char     *name;
};

struct reg_bf {
    enum reg_bf_attr attr;
    uint8_t hi;
    uint8_t low;
    uint32_t def_val;
    uint32_t cur_val;
    struct reg_bf_option options[REG_BF_OPT_MAX_SIZE];
};

struct reg {
    struct reg_group *owner;
    char    *name;
    uint32_t offset;
    uint32_t def_val;
    uint32_t cur_val;
    struct reg_bf bf[REG_BF_MAX_SIZE];
    struct list_head list; /* list on a reg_group */
};

struct reg_group {
    char *name;
    uint32_t base;
    uint32_t end;
    struct list_head grp;
    struct list_head reg;
};

/*****************************************************************************
 * Public Data
 ****************************************************************************/

/*****************************************************************************
 * Inline Functions
 ****************************************************************************/

/****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************/
#ifdef __cplusplus
}
#endif
/****************************************************************************/
#endif /* REG_VIEW_H_INCLUDE */
/****************************************************************************/
/*                                                                          */
/*  End of file.                                                            */
/*                                                                          */
/****************************************************************************/

