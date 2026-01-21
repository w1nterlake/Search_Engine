#include "header.h"
#include "InvertedIndex.h"

struct RelativeIndex{
    int doc_id;
    double rank;
    bool operator == (const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};
class SearchServer {
    public:
    SearchServer(InvertedIndex& indx, int number = 1);   
    std::vector<std::pair<int, std::vector<RelativeIndex>>> search(const std::vector<std::pair<std::string, int>>& queries_input);

    private:
    static bool comparator_2(std::pair<int, std::vector<RelativeIndex>>& first, std::pair<int, std::vector<RelativeIndex>>& second);
    std::vector<std::pair<int, std::vector<RelativeIndex>>> answer;
    static bool comparator(std::pair<int, int>& pair_1, std::pair<int, int>& pair_2);
    void* func(void* arg);
    std::mutex mtx;
    std::condition_variable cond;
    threadpool<SearchServer> t;
    InvertedIndex &indx;
};
