#include "ConverterJSON.h"

Error::Error(std::string str){
    this->str = str;
}

const char* Error::what() const noexcept{
    return str.c_str();
}

ConverterJSON::ConverterJSON(){
    std::ifstream fin;  
    std::ifstream fin_2;
    fin.open("Conf/config.json");
    fin_2.open("Conf/requests.json");
    try{
            if(!fin.is_open()){
                throw Error("config file is missing.");
            }
            fin >> conf;
            if(!conf.contains("config")){
                throw Error("config file is empty.");
            }
            if(conf["config"].contains("name")){
                this->name = conf["config"]["name"];
            }
            if(!conf["config"].contains("version")){
                throw Error("The version of the search engine is not specified.");
            }
            else if(VERSION != conf["config"]["version"]){
                throw Error("config.json has incorrect file version.");
            }
            else{
                this->version = conf["config"]["version"];
            }
            if(!conf["config"].contains("max_responses")){
                max_responses = 1;
            }
            else{
                max_responses = conf["config"]["max_responses"];
            }
            if(!conf["config"].contains("threads")){
                num_threads = 1;
            }
            else{
                num_threads = conf["config"]["threads"];
            }
            if(!conf.contains("files")){
                throw Error("there are no specified files.");
            }
            else{
                for(int i = 0; i < conf["files"].size(); i++){
                    doc_id.push_back(std::make_pair(conf["files"][i], i));
                }
            }
            if(!fin_2.is_open()){
                throw Error("request file is missing.");
            }
            fin_2 >> req;
            if(!req.contains("requests")){
                throw Error("no requests.");
            }
            for(int i = 0; i < req["requests"].size(); i++){
                request_text.push_back(std::make_pair(req["requests"][i], i));
            }
    }
    catch(const Error& e){
        std::cout << e.what();
        fin.close();
        fin_2.close();
        state = true;
        return;
    }

    fin.close();
    fin_2.close();
}

std::vector<std::pair<std::string, int>> ConverterJSON::GetDocumets(){
    return doc_id;
}

std::vector<std::pair<std::string, int>> ConverterJSON::GetRequests(){
    return request_text;
}

int ConverterJSON::GetResponsesLimit(){
    return max_responses;
}

int ConverterJSON::GetNumThread(){
    return num_threads;
}

void ConverterJSON::putAnswers(std::vector<std::pair<int, std::vector<RelativeIndex>>> answers){
    std::string word = "answers";
    ans[word] = js::object();
    for(int i = 0; i < answers.size(); i++){
        if(answers[i].second.size() == 0){
            ans[word][std::string("request") + std::string("0") + std::to_string(i + 1)]["result"] = "false";
        }
        else if(answers[i].second.size() == 1){
            ans[word][std::string("request") + std::string("0") + std::to_string(i + 1)]["result"] = "true";
            ans[word][std::string("request") + std::string("0") + std::to_string(i + 1)]["docid"] = answers[i].second[0].doc_id;
            ans[word][std::string("request") + std::string("0") + std::to_string(i + 1)]["rank"] = answers[i].second[0].rank;
        }
        else{
            // if(i > 9){
            //     ans[word][std::string("request") + std::string("0") + std::to_string(i)]["relevance"] = js::array();
            //     for(int j = 0; j < answers[i].second.size(); j++){
            //         js item;
            //         item["docid"] = answers[i].second[j].doc_id;
            //         item["rank"] = answers[i].second[j].rank; 
            //         ans[word][std::string("request") + std::string("00") + std::to_string(i)]["relevance"].push_back(item);
            //     }
            //     ans[word][std::string("request") + std::string("0") + std::to_string(i)]["result"] = "true";
            // }
            // else{
            //     ans[word][std::string("request") + std::string("00") + std::to_string(i)]["relevance"] = js::array();
            //     for(int j = 0; j < answers[i].second.size(); j++){
            //         js item;
            //         item["docid"] = answers[i].second[j].doc_id;
            //         item["rank"] = answers[i].second[j].rank; 
            //         ans[word][std::string("request") + std::string("00") + std::to_string(i)]["relevance"].push_back(item);
            //     }
            //     ans[word][std::string("request") + std::string("00") + std::to_string(i)]["result"] = "true";
            // }
            ans[word][std::string("request") + std::string("0") + std::to_string(i + 1)]["relevance"] = js::array();
            for(int j = 0; j < answers[i].second.size(); j++){
                js item;
                item["docid"] = answers[i].second[j].doc_id;
                item["rank"] = answers[i].second[j].rank; 
                ans[word][std::string("request") + std::string("0") + std::to_string(i + 1)]["relevance"].push_back(item);
            }
            ans[word][std::string("request") + std::string("0") + std::to_string(i + 1)]["result"] = "true";
            
        }
    }
    std::ofstream fout;
    fout.open("Conf/answers.json");
    try{
        if(!fout.is_open()){
            throw Error("answer file is missing");
        }
    }
    catch(const Error &e){
        std::cout << e.what();
    }
    fout << ans.dump(4);
    fout.close();
}