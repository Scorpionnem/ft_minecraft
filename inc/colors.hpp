//
// Created by bvasseur on 10/7/26.
//

#ifndef MINECRAFT_COLORS_HPP

# include <iostream>

# define RESET	"\033[0m"
# define CLR	"\033[0m"
# define BOLD	"\033[1m"
# define UNDL	"\033[4m"
# define DUNDL	"\033[21m"
# define ITAL	"\033[3m"
# define STRK	"\033[9m"

# define BLK	"\033[0;30m"
# define RED	"\033[0;31m"
# define GRN	"\033[0;32m"
# define YLW	"\033[0;33m"
# define BLU	"\033[0;34m"
# define PRP	"\033[0;35m"
# define CYN	"\033[0;36m"
# define WHT	"\033[0;37m"
# define ORG RGB(255, 92, 0)


# define DBLOCK					"▄"
# define RGB_FG					"\033[38;2"
# define RGB_BG					"\033[48;2"
# define RGB_CODE_STR(str)		(";" + str + ";" + str + ";" + str + "m")
# define RGB_CODE_INT(c)		(";" + std::to_string(c >> 16 & 0xFF) + ";" + std::to_string(c >> 8 & 0xFF) + ";" + std::to_string(c & 0xFF) + "m")
# define RGB_CODE_VEC(v)		(";" + std::to_string(v.x()) + ";" + std::to_string(v.y()) + ";" + std::to_string(v.z()) + "m")
# define RGB_FG_CODE_STR(r,g,b)	(RGB_FG ";" + r + ";" + g + ";" + b + "m")
# define RGB_BG_CODE_STR(r,g,b)	(RGB_BG ";" + r + ";" + g + ";" + b + "m")
# define RGB_FG_CODE(r,g,b)		(RGB_FG ";" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m")
# define RGB_BG_CODE(r,g,b)		(RGB_BG ";" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m")


# define AND <<
# define PRINT std::cout AND
# define PRERR std::cerr AND
# define ENDL AND std::endl
# define CENDL AND CLR ENDL
# define TAB "\t" AND
# define NEWL PRINT "" ENDL;
# define SEMAND AND "; " AND
# define SANDN(name) AND "; " #name ":" AND

#define MINECRAFT_COLORS_HPP

#endif //MINECRAFT_COLORS_HPP