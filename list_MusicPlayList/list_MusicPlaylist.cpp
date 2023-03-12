#include <iostream>
#include <algorithm>

struct Node
{
    std::string song_name;
    Node *prev;
    Node *next;
};

class DoublyLinkedList
{
private:
    using node = Node;
    using node_ptr = node *;

public:
    node_ptr head;
    node_ptr tail;
    node_ptr playing;
    
    void push_back(std::string song_name)
    {
        auto new_node = new node{song_name, nullptr, nullptr};
        if (head == nullptr)
        {
            head = new_node;
        }
        else if (head != nullptr)
        {
            tail->next = new_node;
            new_node->prev = tail;
        }
        tail = new_node;
    }

    // 맨 뒤 노래 삭제
    void pop_back()
    {
        auto last = tail;

        if (head != nullptr)
        {
            tail = tail->prev;
            tail->next = nullptr;
        }
        delete last;
    }

    void remove(std::string _song_name)
    {
        if(head) {
            auto it = begin();
            for (it; it != end(); it++)
            {
                if(*it == _song_name) {
                    auto node = it.Get();
                    if(node == head) {
                        head->next->prev = nullptr;
                        head = head->next;
                        if(playing) playing = nullptr;
                    } else {
                        if(node != tail) {
                            node->next->prev = node->prev;
                        }
                        node->prev->next = node->next;
                        if(playing) playing = node->next;
                    }
                    delete node;
                    return;
                }
            }
        }
    }

    // 반복자 생성
    struct DoublyLinkedListIterator
    {
    private:
        node_ptr ptr;

    public:
        DoublyLinkedListIterator(node_ptr p) : ptr(p) {}

        std::string &operator*() { return ptr->song_name; }
        node_ptr Get() { return ptr; }

        DoublyLinkedListIterator &operator++()
        {
            ptr = ptr->next;
            return *this;
        }

        DoublyLinkedListIterator operator++(int)
        {
            auto result = *this;
            ++(*this);
            return result;
        }

        DoublyLinkedListIterator &operator--()
        {
            ptr = ptr->prev;
            return *this;
        }
        DoublyLinkedListIterator operator--(int)
        {
            auto result = *this;
            --(*this);
            return result;
        }

        friend bool operator==(DoublyLinkedListIterator left, DoublyLinkedListIterator right)
        {
            return left.ptr == right.ptr;
        }

        friend bool operator!=(DoublyLinkedListIterator left, DoublyLinkedListIterator right)
        {
            return left.ptr != right.ptr;
        }
    };

    DoublyLinkedListIterator begin() { return DoublyLinkedListIterator(head); }
    DoublyLinkedListIterator begin() const { return DoublyLinkedListIterator(head); }
    DoublyLinkedListIterator end() { return DoublyLinkedListIterator(nullptr); }
    DoublyLinkedListIterator end() const { return DoublyLinkedListIterator(nullptr); }

    DoublyLinkedList() = default;
    DoublyLinkedList(const DoublyLinkedList &other) : head(nullptr), tail(nullptr), playing(nullptr)
    {
        if (other.head)
        {
            head = new node{NULL, nullptr, nullptr};
            auto cur = head;
            auto it = other.begin();

            while (true)
            {
                cur->song_name = *it;
                auto tmp = it;
                ++tmp;
                if (tmp == other.end())
                {
                    tail = cur;
                    break;
                }
                cur->next = new node{NULL, nullptr, nullptr};
                cur->next->prev = cur;
                cur = cur->next;
                it = tmp;
            }
        }
    }
    DoublyLinkedList(const std::initializer_list<std::string> &ilist) : head(nullptr), tail(nullptr), playing(nullptr)
    {
        auto it = ilist.begin();
        for (it; it != ilist.end(); it++)
        {
            push_back(*it);
        }
    }
};

struct Playlist {
    private:
        DoublyLinkedList* list;
    public:
        Playlist(DoublyLinkedList _list) {
            list = new DoublyLinkedList();
        }
        Playlist(const std::initializer_list<std::string> &ilist) {
            list = new DoublyLinkedList(ilist);
        }

        void new_song(std::string name)
        {
            list->push_back(name);
        }

    void play()
    {
        if(list->head) {
            if (list->playing == nullptr)
                list->playing = list->head;
        }
        std::cout << list->playing->song_name << std::endl;
    }

    void next()
    {
        if (list->playing)
        {
            if (list->playing->next)
                list->playing = list->playing->next;
            else
                list->playing = list->head;
        }
        else
            return;
        std::cout << list->playing->song_name << std::endl;
    }

    void prev()
    {
        if (list->playing)
        {
            if (list->playing->prev)
                list->playing = list->playing->prev;
            else
                list->playing = list->tail;
        }
        else
            return;
        std::cout << list->playing->song_name << std::endl;
    }

    void stop()
    {
        if (list->playing) {
            list->playing == nullptr;
            std::cout << "STOPED" << std::endl;
        }
    }

    void remove(std::string name)
    {
        list->remove(name);
    }

};

int main()
{
    Playlist playlist = {"Hi", "Hello", "World"};
    playlist.play();
    playlist.next();
    playlist.prev();
    playlist.prev();
    playlist.remove("Hi");
    playlist.remove("World");
    playlist.play();
    playlist.stop();
    playlist.play();
    
    return 0;
}

/*
1. 여러 개의 음악을 이용하여 재생 목록을 생성할 수 있습니다.
2. 재생 목록에 음악을 추가할 수 있어야 합니다.
3. 재생 목록에서 음악을 제거할 수 있어야 합니다.
4. 음악을 순환적으로 재생할 수 있어야 합니다(이 기능의 구현은 노래 제목 출력으로 대신합니다)

개발 순서
1. 먼저 원형 데이터 표현을 지원하는 기본 구조를 설계합니다.
2. 앞에서 만든 구조에 데이터 삽입 및 삭제 기능을 구현합니다.
3. 사용자 정의 반복문을 작성합니다. 범위 기반 for 반복문문을 사용하여 모든 원소에 접근할 수 있도록 반복자를 설계해야 합니다.
   따라서 자료구조는 원형이지만 begin()과 end()함수는 각각 다른 주소를 반환해야 합니다.
4. 기본 컨테이너를 만든 후, 이 컨테이너를 기반으로 재생 목록에 여러 음악을 저장하고
   next(), previous(), printfall(), insert(), remove() 같은 함수를 작성합니다.
*/