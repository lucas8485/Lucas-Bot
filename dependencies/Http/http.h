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

#ifndef HTTP_LIBRARY_H
#define HTTP_LIBRARY_H

#include <string>

namespace Http {
    class Request {
    public:
        Request();
        ~Request();
        void setUrl(const std::string&);
        void setMethod(const std::string&);
        void setBody(const std::string&);
        void setHeader(const std::string&);
        void setVerbose(bool verbose);
        std::string getResponse();
        void send();
    private:
        std::string url;
        std::string method;
        std::string body;
        std::string header;
        std::string response;
        bool verbose;
        static size_t writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
    };

    class FileUtilities {
    public:
        static std::string readFile(const std::string& path);
        static void writeFile(const std::string& path, const std::string& content);
    };
}

#endif //HTTP_LIBRARY_H
