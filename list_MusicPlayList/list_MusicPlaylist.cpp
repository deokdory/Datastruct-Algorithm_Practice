#include <iostream>
#include <algorithm>

struct MusicNode
{
    std::string song_name;
    MusicNode *prev;
    MusicNode *next;
};

class MusicPlaylist
{
private:
    using node = MusicNode;
    using node_ptr = node *;

public:
    node_ptr head;
    node_ptr tail;
    node_ptr playing;
    // 맨 뒤에 노래 삽입
    void PushBack(std::string song_name)
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
    void PopBack()
    {
        auto last = tail;

        if (head != nullptr)
        {
            tail = tail->prev;
            tail->next = nullptr;
        }
        delete last;
    }

    void Play()
    {
        if(head) {
            if (playing == nullptr)
                playing = head;
        }
        std::cout << playing->song_name << std::endl;
    }

    void Next()
    {
        if (playing)
        {
            if (playing->next)
                playing = playing->next;
            else
                playing = head;
        }
        else
            return;
        std::cout << playing->song_name << std::endl;
    }

    void Prev()
    {
        if (playing)
        {
            if (playing->prev)
                playing = playing->prev;
            else
                playing = tail;
        }
        else
            return;
        std::cout << playing->song_name << std::endl;
    }

    void Stop()
    {
        if (playing) {
            playing == nullptr;
            std::cout << "STOPED" << std::endl;
        }
    }

    // 반복자 생성
    struct MusicPlaylistIterator
    {
    private:
        node_ptr ptr;

    public:
        MusicPlaylistIterator(node_ptr p) : ptr(p) {}

        std::string &operator*() { return ptr->song_name; }
        node_ptr Get() { return ptr; }

        MusicPlaylistIterator &operator++()
        {
            ptr = ptr->next;
            return *this;
        }

        MusicPlaylistIterator operator++(int)
        {
            auto result = *this;
            ++(*this);
            return result;
        }

        MusicPlaylistIterator &operator--()
        {
            ptr = ptr->prev;
            return *this;
        }
        MusicPlaylistIterator operator--(int)
        {
            auto result = *this;
            --(*this);
            return result;
        }

        friend bool operator==(MusicPlaylistIterator left, MusicPlaylistIterator right)
        {
            return left.ptr == right.ptr;
        }

        friend bool operator!=(MusicPlaylistIterator left, MusicPlaylistIterator right)
        {
            return left.ptr != right.ptr;
        }
    };

    MusicPlaylistIterator begin() { return MusicPlaylistIterator(head); }
    MusicPlaylistIterator begin() const { return MusicPlaylistIterator(head); }
    MusicPlaylistIterator end() { return MusicPlaylistIterator(nullptr); }
    MusicPlaylistIterator end() const { return MusicPlaylistIterator(nullptr); }

    MusicPlaylist() = default;
    MusicPlaylist(const MusicPlaylist &other) : head(nullptr), tail(nullptr), playing(nullptr)
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
    MusicPlaylist(const std::initializer_list<std::string> &ilist) : head(nullptr), tail(nullptr), playing(nullptr)
    {
        auto it = ilist.begin();
        for (it; it != ilist.end(); it++)
        {
            PushBack(*it);
        }
    }
};

int main()
{

    MusicPlaylist base = {"1번곡", "2번곡", "3번곡"};
    base.Play();
    base.Prev();
    base.Next();
    base.Next();
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