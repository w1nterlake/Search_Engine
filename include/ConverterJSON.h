#include "header.h"
#include "SearchServer.h"


using js = nlohmann::json;
#if __linux__
#define PATH "./Conf/"
#elif __WIN32
#define PATH ""
#endif
#define VERSION 1

class Error:public std::exception{
public:
    const char* what() const noexcept override;
    Error(std::string str);
private:
    std::string str;
};

class ConverterJSON{
public:
    ConverterJSON();//+
    std::vector<std::pair<std::string, int>> GetDocumets();//+
    int GetResponsesLimit();//+
    std::vector<std::pair<std::string, int>> GetRequests();
    void putAnswers(std::vector<std::pair<int, std::vector<RelativeIndex>>> answers);
    int GetNumThread();
private:
    js conf;
    js req;
    js ans;
    bool state = false;
    std::string path = PATH;
    int version;
    int max_responses = 0;
    std::string name;
    std::vector<std::pair<std::string, int>> doc_id;
    std::vector<std::pair<std::string, int>> request_text;
    int num_threads = 0;
};

