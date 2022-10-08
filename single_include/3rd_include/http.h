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
