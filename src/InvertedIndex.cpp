#include "InvertedIndex.h"

InvertedIndex::InvertedIndex(int number_thread): t(number_thread){}

void* InvertedIndex::search_map(void* arg){
    wrap<InvertedIndex>* A = (wrap<InvertedIndex>*) arg;
    std::string file_name = A->file_name;
    std::unordered_map<std::string, int> map;
    std::stringstream ss;
    std::string line, word;
    std::ifstream fin(file_name);
    if(!fin.is_open()){
        return nullptr;
    }
    while(std::getline(fin, line)){
        ss = std::stringstream(line);
        while(ss >> word){
            if(map.find(word) != map.end()){
                map[word]++;
            }
            else{
                map[word] = 1;
            }
        }
    }
    mtx.lock();
    for(std::unordered_map<std::string, int>::iterator it = map.begin(); it != map.end(); it++){
        dict[it->first].emplace_back(Entry{A->num, it->second});
    }
    mtx.unlock();

    fin.close();
    return nullptr;
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::pair<std::string, int>> input){
    this->docs = input;
    wrap<InvertedIndex>* tmp_wrap = (wrap<InvertedIndex>*) malloc(input.size() * sizeof(wrap<InvertedIndex>));
    for(int i = 0; i < input.size(); i++){
        new(tmp_wrap + i) wrap<InvertedIndex>;
    }
    for(int i = 0; i < input.size(); i++){
        tmp_wrap[i].file_name = input[i].first;
        tmp_wrap[i].func = &InvertedIndex::search_map;
        tmp_wrap[i].num = i;
        tmp_wrap[i].obj = this;
        t.add(tmp_wrap + i);
    }
    t.work();

    for(int i = 0; i < input.size(); i++){
        tmp_wrap[i].~wrap<InvertedIndex>();
    }
    free(tmp_wrap);
}
void InvertedIndex::SetCountRelev(int n){
    this->count_relev = n;
}

int InvertedIndex::GetCountRelev(){
    return count_relev;
}

std::vector<Entry> InvertedIndex::GetWordCount(std::string word){
    return dict[word];
}

std::vector<std::pair<std::string, int>> InvertedIndex::GetDoc(){
    return docs;
}