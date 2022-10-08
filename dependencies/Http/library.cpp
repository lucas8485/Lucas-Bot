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

#include "library.h"

#include "curl/curl.h"
#include <random>
#include <fstream>
#include <sstream>

namespace Http {
    Request::Request() {
        this->url = "";
        this->method = "GET";
        this->body = "";
        this->header = "";
        this->response = "";
        this->verbose = false;
    }

    Request::~Request() = default;

    void Request::setUrl(const std::string& theUrl) {
        this->url = theUrl;
    }

    void Request::setMethod(const std::string& theMethod) {
        this->method = theMethod;
    }

    void Request::setBody(const std::string& theBody) {
        this->body = theBody;
    }

    void Request::setHeader(const std::string& theHeader) {
        this->header = theHeader;
    }

    void Request::setVerbose(bool theVerbose) {
        this->verbose = theVerbose;
    }

    std::string Request::getResponse() {
        return this->response;
    }

    void Request::send() {
        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, this->url.c_str());
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, this->method.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, this->body.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Request::writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &this->response);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, this->verbose);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
    }

    size_t Request::writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
        auto *response = static_cast<std::string *>(userdata);
        response->append(ptr, size * nmemb);
        return size * nmemb;
    }


    std::string FileUtilities::readFile(const std::string &path) {
        std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
        std::stringstream buffer;
        // read binary from path to the string
        buffer << file.rdbuf();
        return buffer.str();
    }

    void FileUtilities::writeFile(const std::string &path, const std::string &content) {
        std::ofstream file(path, std::ios::out | std::ios::binary);
        // write content to path in binary format
        file.write(content.c_str(), content.size());
        // Close
        file.close();
    }
}
