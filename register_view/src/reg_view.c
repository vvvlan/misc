/*****************************************************************************
 * reg_view.c
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

/*****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reg_view.h"

/*****************************************************************************
 * Trace Definitions
 ****************************************************************************/

#define TRACE_LEVEL   TRACE_LEVEL_DEBUG
#define TRACE_DOMAIN  "reg_view"
#include "sys_trace.h"

/*****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/*****************************************************************************
 * Private Types
 ****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Data
 ****************************************************************************/

LIST_HEAD(reg_grp_list);

/*****************************************************************************
 * Public Data
 ****************************************************************************/

/*****************************************************************************
 * Private Functions
 ****************************************************************************/

/*****************************************************************************
 * Public Functions
 ****************************************************************************/

struct reg_group *reg_group_add(char *name, uint32_t base, uint32_t end)
{
    struct reg_group *grp;

    grp = malloc(sizeof(struct reg_group));

    if (grp != NULL) {
        grp->name = strdup(name);
        grp->base = base;
        grp->end  = end;

        INIT_LIST_HEAD(&grp->grp);
        INIT_LIST_HEAD(&grp->reg);
        list_add(&grp->grp, &reg_grp_list);
    }

    return grp;
}

void reg_group_del(struct reg_group *grp)
{
    if (grp == NULL) {
        return ;
    }

    free(grp->name);
    list_del(&grp->grp);
    free(grp);
}

struct reg_group *reg_group_find_by_addr(uint32_t addr)
{
    struct reg_group *grp_entry;

    list_for_each_entry(grp_entry, &reg_grp_list, grp) {
        if (addr >= grp_entry->base && addr <= grp_entry->end) {
            T_D("%-s 0x%08X-0x%08X", grp_entry->name, grp_entry->base, grp_entry->end);
            return grp_entry;
        }
    }

    return NULL;
}



int main(void)
{
    reg_group_add("hic", 0x00020000, 0x00021000);

    reg_group_find_by_addr(0x00020000);

    return 0;
}


/****************************************************************************/
/*                                                                          */
/*  End of file.                                                            */
/*                                                                          */
/****************************************************************************/

