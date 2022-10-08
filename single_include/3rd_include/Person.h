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
#ifndef PERSON_DEMO_PERSON_H
#define PERSON_DEMO_PERSON_H

#include <ctime>
#include "json.hpp"

namespace MiraiCP {
    using QQID = unsigned long long;
}

class Person {
private:
    MiraiCP::QQID qqID;
    // 上次签到时间
    std::time_t lastSignOnTime;
    // 连续签到天数
    int signOnDays;
    // 发言未被攻击累计次数
    int speakCount;
    // 铜钱数量
    int copperCoin;
public:
    // Generate ctors and dtors
    // assignment operators
    Person() = default;
    explicit Person(MiraiCP::QQID id);

    // Deserialize from JSON
    explicit Person(const nlohmann::json&);
    ~Person() = default;
    Person(const Person&) = default;
    Person& operator=(const Person&) = default;
    Person(Person&&) = default;
    Person& operator=(Person&&) = default;

    [[nodiscard]] bool isSignOnToday() const;
    void signOn();
    [[nodiscard]] int continuousSignOnDays() const;

    [[nodiscard]] bool willBeAttackNextTime() const;
    void resetAttackCount();
    void increaseSpeakCount();

    [[nodiscard]] int getCopperCoin() const;
    nlohmann::json serializeToJSON();

    [[nodiscard]] MiraiCP::QQID getQQID() const;
};


#endif //PERSON_DEMO_PERSON_H
