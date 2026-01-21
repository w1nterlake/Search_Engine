#include "SearchServer.h"

SearchServer::SearchServer(InvertedIndex& i, int number):indx(i), t(number){}

bool SearchServer::comparator_2(std::pair<int, std::vector<RelativeIndex>>& first, std::pair<int, std::vector<RelativeIndex>>& second){
    if(first.first < second.first){
        return true;
    }
    else{
        return false;
    }
}

bool SearchServer::comparator(std::pair<int, int>& pair_1, std::pair<int, int>& pair_2){
    if(pair_1.second > pair_2.second){
        return true;
    }
    else if(pair_1.second == pair_2.second){
        if(pair_1.first < pair_2.first){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

void* SearchServer::func(void* arg){
    wrap<SearchServer>* A = (wrap<SearchServer>*) arg;
    std::string file = A->file_name;
    std::unordered_set<std::string> set;
    std::stringstream ss;
    std::string word, line;
    std::vector<Entry> tmp;
    std::vector<std::pair<int, int>> d;
    for(int i = 0; i < indx.GetDoc().size(); i++){
        d.emplace_back(std::make_pair(i, 0));
    }
        ss = std::stringstream(file);
    while(ss >> word){
        if(set.find(word) == set.end()){
            set.insert(word);
            tmp = indx.GetWordCount(word);
            for(int i = 0; i < tmp.size(); i++){
                d[tmp[i].doc_id].second += tmp[i].count;
            }
        }
    }
    //осталось посчитать релевантность и собрать всё в кучу
    int min = indx.GetCountRelev() < indx.GetDoc().size() ? indx.GetCountRelev() : indx.GetDoc().size();
    std::partial_sort(d.begin(), d.begin() + min, d.end(), comparator);
    std::vector<RelativeIndex> vec;
    for(int i = 0; i < min; i++){
        if(d[0].second == 0){
            vec.emplace_back(RelativeIndex{d[i].first, 0});            
        } 
        else{
            double num = (double)d[i].second / (double)d[0].second;
            num = (double)std::round(num * 10.) / 10.;
            vec.emplace_back(RelativeIndex{d[i].first, num});
        }
    }
    mtx.lock();
    answer.push_back(std::make_pair(A->num, vec));
    mtx.unlock();

    return nullptr;
}

std::vector<std::pair<int, std::vector<RelativeIndex>>> SearchServer::search(const std::vector<std::pair<std::string, int>>& queries_input){
    wrap<SearchServer>* tmp_wrap = (wrap<SearchServer>*) malloc(queries_input.size() * sizeof(wrap<SearchServer>));
    int req_size = queries_input.size();
    for(int i = 0; i < req_size; i++){
        new (tmp_wrap + i) wrap<SearchServer>;
    }
    for(int i = 0; i < req_size; i++){
        tmp_wrap[i].file_name = queries_input[i].first;
        tmp_wrap[i].func = &SearchServer::func;
        tmp_wrap[i].num = queries_input[i].second;
        tmp_wrap[i].obj = this;
        t.add(tmp_wrap + i);
    }
    t.work();

    std::sort(answer.begin(), answer.end(), comparator_2);

    for(int i = 0; i < req_size; i++){
        tmp_wrap[i].~wrap();
    }
    free(tmp_wrap);
    return answer;
}