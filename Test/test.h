// #include "gtest/gtest.h"
// #include "InvertedIndex.h"

// void TestInvertedIndex(std::vector<std::pair<std::string, int>> docs, std::vector<std::string> req, std::vector<std::vector<Entry>> expected){
//     std::vector<std::vector<Entry>> result;
//     InvertedIndex indx(1);
//     indx.UpdateDocumentBase(docs);
//     for(int i = 0; i < req.size(); i++){
//         std::string word;
//         int count = 0;
//         std::vector<Entry> tmp = indx.GetWordCount(req[i]);
//         result.push_back(tmp);
//     }
//     ASSERT_EQ(result, expected);
// }

// TEST(sample_test_case, sample_test)
// {
//     const std::vector<std::pair<std::string, int>> docs = {
//         {"../../Test/test_1_1.txt", 0},
//         {"../../Test/test_1_2.txt", 1}
//     };
//     const std::vector<std::string> requests = {"london", "the"};
//     const std::vector<std::vector<Entry>> expected = {
//         {
//                 {0, 1}
//         }, {
//                 {0, 1}, {1, 3}
//         }
//     };
//     TestInvertedIndex(docs, requests, expected);
// }

// TEST(TestCaseInvertedIndex, TestBasic2) {
//     std::vector<std::pair<std::string, int>> docs = {
//         {"..//..//Test//test_1_3.txt", 0},
//         {"..//..//Test//test_1_4.txt", 1},
//         {"..//..//Test//test_1_5.txt", 2},
//         {"..//..//Test//test_1_6.txt", 3}
// };
//     const std::vector<std::string> requests = {"milk", "water", "cappuccino"};
//     const std::vector<std::vector<Entry>> expected = {
//         {
//             {0, 4}, {1, 1}, {2, 5}
//         }, {
//                         {0, 3}, {1, 2}, {2, 5}
//         }, {
//                         {3, 1}
//         }
//     };
//     TestInvertedIndex(docs, requests, expected);
// }


// TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
//     const std::vector<std::pair<std::string, int>> docs = {
//         {"..//..//Test//test_1_7.txt", 0},
//         {"..//..//Test//test_1_8.txt", 1}
// };
//     const std::vector<std::string> requests = {"m", "statement"};
//     const std::vector<std::vector<Entry>> expected = {
//         {
//         }, {
//                         {1, 1}
//         }
//     };
//     TestInvertedIndex(docs, requests, expected);
// }