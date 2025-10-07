/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:29:27 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/07 12:03:47 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBS_HPP
# define LIBS_HPP

# include "strings.hpp"

# include <iostream>
# include <iomanip>
# include <fstream>
# include <sstream>
# include <istream>

# include <string>
# include <vector>
# include <map>
# include <unordered_map>
# include <array>
# include <deque>

# include <exception>

# include <cmath>
# include <functional>
# include <algorithm>
# include <filesystem>

# include <thread>
# include <mutex>
# include <atomic>

# include <unistd.h>

# include "glad.h"
# include "glfw3.h"

# include "stb_image.h"
# include "gtc/matrix_transform.hpp"
# include "gtc/type_ptr.hpp"

# include "json.h"
using json = nlohmann::json;

#endif
