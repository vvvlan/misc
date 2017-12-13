/*****************************************************************************
 * sys_trace.h
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

/*
 * Config marco
 * - CONFIG_TRACE: enable trace
 * - CONFIG_TRACE_DEFAULT_LEVEL: default trace level
 * - CONFIG_TRACE_SHOW_COLOR: Should use color(red-error, yellow-warning)
 * - CONFIG_TRACE_SHOW_TAGS: Should use level tags
 *
 * how to use
 * #define TRACE_LEVEL   TRACE_LEVEL_DEBUG
 * #define TRACE_DOMAIN  "test"
 * //#define TRACE_NO_NEWLINE
 * //#define CONFIG_TRACE_NO_TAGS
 * #include "sys_trace.h"
 *
 */

#ifndef SYS_TRACE_H_INCLUDE
#define SYS_TRACE_H_INCLUDE
/*****************************************************************************
 * Included Files
 ****************************************************************************/
/*****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define CONFIG_TRACE_SHOW_COLOR
#define CONFIG_TRACE_SHOW_TAGS

/* Trace Level */
#define TRACE_LEVEL_OFF     0
#define TRACE_LEVEL_ERROR   1 /* T_E */
#define TRACE_LEVEL_WARNING 2 /* T_W */
#define TRACE_LEVEL_INFO    3 /* T_I */
#define TRACE_LEVEL_DEBUG   4 /* T_D */

/* Determine this compile unit trace level */
#if !defined(TRACE_LEVEL)
#define TRACE_LEVEL  TRACE_LEVEL_ERROR
#endif

#if (TRACE_LEVEL > TRACE_LEVEL_OFF)

/* decide print func */
#if !defined(TRACE_BACKEND_FN)
#define TRACE_BACKEND_FN printf
#endif

#if !defined(TRACE_NO_NEWLINE)
#define TRACE_NL "\n"
#else
#define TRACE_NL ""
#endif

/* Should use color? */
#if defined(CONFIG_TRACE_SHOW_COLOR)
#define TRACE_COLOR_OFF     "\x1B[0m"
#define TRACE_COLOR_RED     "\x1B[0;31m"
#define TRACE_COLOR_YELLOW  "\x1B[0;33m"
#else
#define TRACE_COLOR_OFF     ""
#define TRACE_COLOR_RED     ""
#define TRACE_COLOR_YELLOW  ""
#endif /* CONFIG_TRACE_SHOW_COLOR */

/* Should use level tags? */
#if defined(CONFIG_TRACE_SHOW_TAGS)
#define TRACE_TAG_ERR "/E"
#define TRACE_TAG_WRN "/W"
#define TRACE_TAG_INF "/I"
#define TRACE_TAG_DBG "/D"
#else
#define TRACE_TAG_ERR ""
#define TRACE_TAG_WRN ""
#define TRACE_TAG_INF ""
#define TRACE_TAG_DBG ""
#endif /* CONFIG_TRACE_SHOW_TAGS */

/* trace domain name */

#if !defined(TRACE_DOMAIN)
#define TRACE_DOMAIN "general"
#endif /* TRACE_DOMAIN */

/* [domain] [level] [function:line]: */
#define TRACE_LAYOUT "[%-10s] %s %-12s(%04d) %s"
#define TRACE_BACKEND_CALL(level, color, format, color_off, ...)	         \
	TRACE_BACKEND_FN(TRACE_LAYOUT format "%s" TRACE_NL,	                     \
	                 TRACE_DOMAIN,                                           \
	                 level, __func__, __LINE__, color, ##__VA_ARGS__, color_off)

#define TRACE_NO_COLOR(level, format, ...)				                     \
	TRACE_BACKEND_CALL(level, "", format, "", ##__VA_ARGS__)
#define TRACE_COLOR(level, color, format, ...)			                     \
	TRACE_BACKEND_CALL(level, color, format, TRACE_COLOR_OFF, ##__VA_ARGS__)


#define T_E(...) TRACE_COLOR(TRACE_TAG_ERR, TRACE_COLOR_RED, ##__VA_ARGS__)

#if (TRACE_LEVEL >= TRACE_LEVEL_WARNING)
#define T_W(...) TRACE_COLOR(TRACE_TAG_WRN, TRACE_COLOR_YELLOW, ##__VA_ARGS__)
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_INFO)
#define T_I(...) TRACE_NO_COLOR(TRACE_TAG_INF, ##__VA_ARGS__)
#endif /* #if (TRACE_LEVEL >= TRACE_LEVEL_INFO) */

#if (TRACE_LEVEL >= TRACE_LEVEL_DEBUG)
#define T_D(...) TRACE_NO_COLOR(TRACE_TAG_DBG, ##__VA_ARGS__)
#define T_D_HEX(name, byte_p, byte_cnt)                                      \
    do {                                                                     \
        uint16_t i = 0, j = 0;                                               \
        TRACE_BACKEND_FN("%s\n", name);                                     \
        while (i < byte_cnt) {                                               \
            j = 0;                                                           \
            TRACE_BACKEND_FN("%08X/%08X:", (uint32_t)byte_p+i, i);           \
            while (j+i < byte_cnt && j < 16) {                               \
                if (j % 2 == 0) {                                            \
                    TRACE_BACKEND_FN("%c", ' ');                             \
                }                                                            \
                TRACE_BACKEND_FN("%02X", byte_p[i+j]);                       \
                j++;                                                         \
            }                                                                \
            while (j < 16) {                                                 \
                if (j % 2 == 0) {                                            \
                    TRACE_BACKEND_FN("%c", ' ');                             \
                }                                                            \
                TRACE_BACKEND_FN("%s", "  ");                                \
                j++;                                                         \
            }                                                                \
            TRACE_BACKEND_FN("%s", " @ ");                                   \
            j = 0;                                                           \
            while (j+i < byte_cnt && j < 16) {                               \
                TRACE_BACKEND_FN("%c",                                       \
                    isprint(byte_p[i+j]) ? byte_p[i+j] : '.');               \
                j++;                                                         \
            }                                                                \
            TRACE_BACKEND_FN("%s", "\n");                                    \
            i += 16;                                                         \
        }                                                                    \
        TRACE_BACKEND_FN("\n");                                              \
    } while (0)
#endif /* #if (TRACE_LEVEL >= TRACE_LEVEL_DEBUG) */

#endif

/* create dummy macros */
#if !defined(T_E)
#define T_E(...) do { } while (0)
#endif

#if !defined(T_W)
#define T_W(...) do { } while (0)
#endif

#if !defined(T_I)
#define T_I(...) do { } while (0)
#endif

#if !defined(T_D)
#define T_D(...) do { } while (0)
#endif

#if !defined(T_D_HEX)
#define T_D_HEX(...) do { } while (0)
#endif

/*****************************************************************************
 * Public Types
 ****************************************************************************/
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
#endif /* SYS_TRACE_H_INCLUDE */
/****************************************************************************/
/*                                                                          */
/*  End of file.                                                            */
/*                                                                          */
/****************************************************************************/
