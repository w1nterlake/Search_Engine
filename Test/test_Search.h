#include "gtest/gtest.h"
#include "InvertedIndex.h"
#include "SearchServer.h"


TEST(TestCaseSearchServer, TestSimple) {
    const std::vector<std::pair<std::string, int>> docs = {
        {"Test//test_doc_1_1.txt", 0},
        {"Test//test_doc_1_2.txt", 1},
        {"Test//test_doc_1_3.txt", 2},
        {"Test//test_doc_1_4.txt", 3}
};
    const std::vector<std::pair<std::string, int>> request = {{"milk water", 0}, {"sugar", 1}};
    const std::vector<std::vector<RelativeIndex>> expected = {
        {
            {2, 1.0},
            {0, 0.7},
            {1, 0.3},
            {3, 0.0}
        },
        { 
            {0, 0.0},
            {1, 0.0},
            {2, 0.0},
            {3, 0.0}
        }
    };
    InvertedIndex idx(1);
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<std::pair<int, std::vector<RelativeIndex>>> result = srv.search(request);
    std::vector<std::vector<RelativeIndex>> tmp;
    for (int i = 0; i < result.size(); i++) {
        tmp.push_back(result[i].second);
    }
    ASSERT_EQ(tmp, expected);
}