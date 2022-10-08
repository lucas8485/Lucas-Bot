/*
 *  This file is part of Lucas' Bot.
 *
 *  Lucas' Bot is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License,
 *  or (at your option) any later version.

    Lucas' Bot is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
    for more details.

    You should have received a copy of the GNU Affero General Public License along
    with Lucas' Bot. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PERSON_DEMO_PERSONCOLLECTION_H
#define PERSON_DEMO_PERSONCOLLECTION_H


#include <unordered_map>
#include "Person.h"
#include <string>

namespace PersonCollection {
    void loadPersonMap();
    void savePersonMap();
    void addPerson(MiraiCP::QQID id);
    Person& getPerson(MiraiCP::QQID id);
    void removePerson(MiraiCP::QQID id);
    void registerLocalFile(const std::string&);
}



#endif //PERSON_DEMO_PERSONCOLLECTION_H
