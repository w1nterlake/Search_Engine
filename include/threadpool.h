#ifndef HEADER_H2
#define HEADER_H2
#include "header.h"


template<typename T>
struct wrap{
    T* obj = nullptr;
    size_t num = 0;
    std::string file_name;
    void* (T::*func)(void*) = nullptr;
};

template<>
struct wrap<void>{
    size_t num = 0;
    std::string file_name;
    void (*func)(void*) = nullptr;
};

template<typename T>
struct Node{
    Node* next = nullptr;
    wrap<T>* data = nullptr;
};

template<typename T>
class Queue{
    private:
    Node<T>* head = nullptr;
    Node<T>* tail = nullptr;
    int size = 0;
    public:
    ~Queue();
    void push(wrap<T>* tmp);
    wrap<T>* get();
    int get_size();
};

template<typename T>
class threadpool{
    private:
    double t = 0;
    bool flag = false;
    int n;
    std::thread *th;
    Queue<T> q;
    std::mutex mtx;
    std::condition_variable cv;
    public:
    void add(wrap<T>* w);
    void work();
    threadpool(int count);
    ~threadpool();
};

template<typename T>
void Queue<T>::push(wrap<T>* w){
    Node<T>* tmp = new Node<T>;
    tmp->data = w;
    if(size == 0 && head == nullptr && tail == nullptr){
        head = tmp;
        head->next = nullptr;
        tail = head;
        size++;
    }
    else if(size == 1 && tail == head){
        head = tmp;
        head->next = tail;
        size++;
    }
    else{
        tmp->next = head;
        head = tmp;
        size++;
    }
}

template<typename T>
wrap<T>* Queue<T>::get(){
    if(size == 0 && head == nullptr && tail == nullptr){
        return nullptr;
    }
    else if(size == 1 && head == tail){
        size--;
        wrap<T>* tmp;
        tmp = tail->data;
        head->next = nullptr;
        head->data = nullptr;
        delete head;
        head = nullptr;
        tail = nullptr;
        return tmp;
    }
    else{
        size--;
        wrap<T>* tmp = tail->data;
        Node<T>* temp = head;
        while(temp->next != tail){
            temp = temp->next;
        }
        tail->data = nullptr;
        tail->next = nullptr;
        delete tail;
        tail = temp;
        tail->next = nullptr;
        return tmp;
    }
    return nullptr;
}

template <typename T>
int Queue<T>::get_size(){
    return size;
}

template<typename T>
Queue<T>::~Queue(){
    Node<T>* tmp = head;
    while(true){
        if(size == 0 && head == nullptr && tail == nullptr){
            break;
        }
        head = tmp->next;
        tmp->data = nullptr;
        tmp->next = nullptr;
        delete tmp;
        size--;
    }
}

template<typename T>
void threadpool<T>::add(wrap<T>* w){
   q.push(w);
}

template<typename T>
threadpool<T>::threadpool(int count){
    n = count;
    th = (std::thread*)malloc(count * sizeof(std::thread));
}

template<typename T>
threadpool<T>::~threadpool(){
    std::cout << (clock() - t)/CLOCKS_PER_SEC << std::endl;
    for(int i = 0; i < n; i++){
        th[i].~thread();
    }
    free(th);
}

template<typename T>
void threadpool<T>::work(){
    t = clock();
    for(int i = 0; i < n; i++){
        new (th + i) std::thread([this](){
            while(true){
                wrap<T>* w;
                mtx.lock();
                    if(q.get_size() == 0){
                        mtx.unlock();
                        return;
                    }
                    w = this->q.get();
                mtx.unlock();
                (w->obj->*(w->func))((void*)w);
            }
        });
    }
    for(int i = 0; i < n; i++){
        th[i].join();
    }
}


#endif