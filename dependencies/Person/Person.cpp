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

#include "Person.h"
#include <random>

bool Person::isSignOnToday() const {
    // Get today's 12:00 A.M. time point
    std::time_t now = std::time(nullptr);
    std::tm* nowTm = std::localtime(&now);
    nowTm->tm_hour = 0;
    nowTm->tm_min = 0;
    nowTm->tm_sec = 0;
    std::time_t today = std::mktime(nowTm);

    // Check last sign on time
    // And compare it with today's 12:00 A.M.
    // If last sign on time is before today's 12:00 A.M.,
    // Then it is not sign on today
    return lastSignOnTime >= today;
}

void Person::signOn() {
    if (isSignOnToday()) {
        return;
    }
    // Check last sign-on date
    // If it is yesterday, add 1 to signOnDays
    // If it is not yesterday, reset signOnDays to 1
    if (std::time(nullptr) - lastSignOnTime < 2 * 24 * 60 * 60) {
        signOnDays++;
    } else {
        signOnDays = 1;
    }
    lastSignOnTime = std::time(nullptr);
    // Increase Copper coin amount by random
    // Range: 0-100
    // Use Normal Distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dis(50, 20);
    copperCoin += static_cast<int>(dis(gen));
}

int Person::continuousSignOnDays() const {
    // Check last sign-on date
    // If it's not yesterday, return 0
    // If it is yesterday or today, return signOnDays

    // Get Yesterday's 12:00 A.M. Time Point
    std::time_t now = std::time(nullptr);
    std::tm* nowTm = std::localtime(&now);
    nowTm->tm_hour = 0;
    nowTm->tm_min = 0;
    nowTm->tm_sec = 0;
    std::time_t today = std::mktime(nowTm);
    std::time_t yesterday = today - 24 * 60 * 60;

    // Check last sign on time
    // And compare it with yesterday's 12:00 A.M.
    // If last sign on time is before yesterday's 12:00 A.M.,
    // Then it is not sign on yesterday, returns zero
    // and set signOnDays to 0
    // else, returns signOnDays
    if (lastSignOnTime < yesterday) {
        return 0;
    } else {
        return signOnDays;
    }
}

bool Person::willBeAttackNextTime() const {
    static std::default_random_engine e(std::time(nullptr));
    std::uniform_int_distribution<unsigned> u(0, 10000);
    // u(e) divide 100 is the probability
    switch (speakCount) {
        // Case 0-73: the probability is 0.60%
        case 0 ... 73:
            return u(e) < 60;
            // Case 74-90:
            // The base probability is 6.60%
            // Increase by 6% for each 1 times
            // Max probability is 100%
        case 74 ... 90:
            return u(e) < 660 + 60 * (speakCount - 74);
        // If speakCount > 90, the probability is 100%
        default:
            return true;
    }
}

void Person::resetAttackCount() {
    speakCount = 0;
}

void Person::increaseSpeakCount() {
    speakCount++;
}

// Serialize to JSON
nlohmann::json Person::serializeToJSON() {
    nlohmann::json j;
    j["qqID"] = qqID;
    j["lastSignOnTime"] = lastSignOnTime;
    j["signOnDays"] = signOnDays;
    j["speakCount"] = speakCount;
    j["copperCoin"] = copperCoin;
    return j;
}

// Deserialize from JSON
Person::Person(const nlohmann::json &json) :
        qqID(json["qqID"]),
        lastSignOnTime(json["lastSignOnTime"]),
        signOnDays(json["signOnDays"]),
        speakCount(json["speakCount"]),
        copperCoin(json["copperCoin"]) {}

int Person::getCopperCoin() const {
    return copperCoin;
}

MiraiCP::QQID Person::getQQID() const {
    return qqID;
}

Person::Person(MiraiCP::QQID id) :
        qqID(id), lastSignOnTime(0), signOnDays(0), speakCount(0), copperCoin(0) {}
