#ifndef HEADER_H1
#define HEADER_H1
#include "header.h"
#include "threadpool.h"

struct Entry{
    size_t doc_id;
    int count = 0;
    bool operator == (const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

class InvertedIndex{
public:
    InvertedIndex(int number_thread = 1);//+
    void UpdateDocumentBase(std::vector<std::pair<std::string, int>> input);
    std::vector<Entry> GetWordCount(std::string word);
    std::vector<std::pair<std::string, int>> GetDoc();
    void SetCountRelev(int n);
    int GetCountRelev();
private:
    int count_relev;
    std::vector<std::pair<std::string, int>> docs;
    std::unordered_map<std::string, std::vector<Entry>> dict;
    threadpool<InvertedIndex> t;
    std::mutex mtx;
    void* search_map(void* arg);
};


#endif