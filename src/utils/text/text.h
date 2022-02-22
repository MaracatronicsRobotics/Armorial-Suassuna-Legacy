/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef TEXT_H
#define TEXT_H

#include <iostream>

class Text
{
public:
    Text();

    static std::string center(std::string s);
    static std::string bold(std::string s);
    static std::string red(std::string s, bool bold = false);
    static std::string green(std::string s, bool bold = false);
    static std::string yellow(std::string s, bool bold = false);
    static std::string blue(std::string s, bool bold = false);
    static std::string purple(std::string s, bool bold = false);
    static std::string cyan(std::string s, bool bold = false);
};

#endif // TEXT_H
