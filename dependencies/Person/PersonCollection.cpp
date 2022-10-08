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
#include "PersonCollection.h"
#include <fstream>
#include <filesystem>
#include <thread>
#include <mutex>
#include <chrono>


std::string localFilePath;

namespace PersonCollection {

    std::unordered_map<MiraiCP::QQID, Person> personMap;
    std::mutex personMapMutex;
    bool initialized = false;

    [[noreturn]]
    void savePersonMapPeriodicallyWorker(const std::string &path);

    // Refactoring:
    // loadPersonMap and savePersonMap are now using
    //    globally defined localFilePath
    void loadPersonMap() {
        // If localFilePath is not defined
        if (initialized || localFilePath.empty() || !std::filesystem::exists(localFilePath)) {
            return;
        }
        std::ifstream ifs(localFilePath);
        nlohmann::json j;
        // j is a json array containing person
        ifs >> j;
        // Lock the mutex
        std::lock_guard<std::mutex> lock(personMapMutex);
        for (auto &person : j) {
            personMap[person["qqID"]] = Person(person);
        }
        // Start a thread to save the map periodically
        std::thread t(savePersonMapPeriodicallyWorker, localFilePath);
        t.detach();
        initialized = true;
    }

    void savePersonMap() {
        // If localFilePath is not defined
        if (localFilePath.empty()) {
            // Throw an exception
            throw std::runtime_error("Local file path is not defined");
        }
        std::ofstream ofs(localFilePath);
        if (!ofs.is_open()) {
            return;
        }
        nlohmann::json j;
        {
            // Lock the mutex
            std::lock_guard<std::mutex> lock(personMapMutex);
            for (auto &person : personMap) {
                j.push_back(person.second.serializeToJSON());
            }
        }
        ofs << j.dump(4);
    }

    void addPerson(MiraiCP::QQID id) {
        // Lock the mutex
        std::lock_guard<std::mutex> lock(personMapMutex);
        personMap[id] = Person(id);
    }

    Person& getPerson(MiraiCP::QQID id) {
        if (personMap.find(id) == personMap.end())
            personMap.insert({id, Person(id)});
        return personMap[id];
    }

    void removePerson(MiraiCP::QQID id) {
        // Lock the mutex
        std::lock_guard<std::mutex> lock(personMapMutex);
        personMap.erase(id);
    }

    void savePersonMapPeriodically(const std::string &path) {
        std::thread t(savePersonMapPeriodicallyWorker, path);
        t.detach();
    }

    void savePersonMapPeriodicallyWorker(const std::string &path) {
        while (true) {
            std::this_thread::sleep_for(std::chrono::minutes(1));
            savePersonMap();
        }
    }

    void registerLocalFile(const std::string &path) {
        localFilePath = path;
    }
}