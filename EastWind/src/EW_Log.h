/*  Log.h file <migrated from LogSystem.h>
 *  * Created by Langyan Zang
 *
 *  * This file is intended to provide colorful output to terminal APIs, or more formmally macros
 *  
 *  * The following are some hints for the design of colorful output using ANSI ESCAPE Sequences
 *    start with "\033" -> ESC character and "[", i.e. "\033[", followed by a format and color seperated by ";" and a letter "m".
 *    end with the same pattern as above.
 *
 *                    foreground background
 *  black                 30         40
 *  red                   31         41
 *  green                 32         42
 *  yellow                33         43
 *  blue                  34         44
 *  magenta               35         45
 *  cyan                  36         46
 *  white                 37         47
 *  gray(bright black)    90         100
 *  bright red            91         101
 *  bright green          92         102
 *  bright yellow         93         103
 *  bright blue           94         104
 *  bright magenta        95         105
 *  bright cyan           96         106
 *  bright white          97         107
 *
 *  reset             0  (everything back to normal)
 *  bold/bright       1  (often a brighter shade of the same colour)
 *  itatlic           3
 *  underline         4
 *  slow blink        5  (not supported in some terminal)
 *  rapid blink       6  (not supported in some terminal)
 *  inverse           7  (swap foreground and background colours)
 *  bold/bright off  21
 *  underline off    24
 *  inverse off      27
 *
 *  * For a full list and support system, check https://en.wikipedia.org/wiki/ANSI_escape_code
 *  and SGR table https://en.wikipedia.org/wiki/ANSI_escape_code#SGR_(Select_Graphic_Rendition)_parameters
 *
 *  Enjoy Logging!
 */


#ifndef LOG_H
#define LOG_H

#include "EW_pch.h"

#define LOG(s, pat, color) std::cout << "\033[" << pat << color << "m" << s << "\033[0m\n" 

#define EW_CORE_TRACE(s) LOG(s, "1;", "34") 
#define EW_CORE_INFO(s)  LOG(s, "1;", "92") 
#define EW_CORE_WARN(s)  LOG(s, "1;7;", "93") 
#define EW_CORE_ERROR(s) LOG(s, "1;7;", "91") 
#define EW_CORE_FATAL(s) LOG(s, "1;7;", "91;106") 

#define EW_TRACE(s) LOG(s, "1;", "34") 
#define EW_INFO(s)  LOG(s, "1;", "92") 
#define EW_WARN(s)  LOG(s, "1;7;", "93") 
#define EW_ERROR(s) LOG(s, "1;7;", "91") 
#define EW_FATAL(s) LOG(s, "1;7;", "91;106") 



#endif
