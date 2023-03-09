#include <iostream>
#include <algorithm>

struct singly_ll_node
{
    int data;
    singly_ll_node *next;
};

class singly_ll
{
public:
    using node = singly_ll_node;
    using node_ptr = node *;

private:
    node_ptr head;

public:
    void push_front(int val)
    {
        auto new_node = new node{val, nullptr};
        if (head != nullptr)
            new_node->next = head;
        head = new_node;
    }

    void pop_front()
    {
        auto first = head;
        if (head)
        {
            head = head->next;
            delete first;
        }
    }

    // 반복자 구현 (생성자와 접근자를 가짐)
    struct singly_ll_iterator
    {
    private:
        node_ptr ptr;

    public:
        singly_ll_iterator(node_ptr p) : ptr(p) {}

        int &operator*() { return ptr->data; }

        node_ptr get() { return ptr; }

        singly_ll_iterator &operator++() // 선행 증가
        {
            ptr = ptr->next;
            return *this;
        }

        singly_ll_iterator operator++(int) // 후행 증가
        {
            singly_ll_iterator result = *this;
            ++(*this);
            return result;
        }

        friend bool operator==(const singly_ll_iterator &left, const singly_ll_iterator &right)
        {
            return left.ptr == right.ptr;
        }

        friend bool operator!=(const singly_ll_iterator &left, const singly_ll_iterator &right)
        {
            return left.ptr != right.ptr;
        }
    };

    // begin() & end() 함수 ( end() 함수는 지원하지 않으므로 nullptr 반환?
    // 추측 : 어차피 for문 돌릴 시 i != singly_ll.end() 와 같은 조건일테니 nullptr를 반환하는 듯
    
    singly_ll_iterator begin() { return singly_ll_iterator(head); }
    singly_ll_iterator end() { return singly_ll_iterator(nullptr); }
    singly_ll_iterator begin() const { return singly_ll_iterator(head); }
    singly_ll_iterator end() const { return singly_ll_iterator(nullptr); }
    

    // 기본 생성자
    singly_ll() = default;

    // 깊은 복사를 위한 복사 생성자
    singly_ll(const singly_ll &other) : head(nullptr)
    {
        if (other.head)
        {
            head = new node{0, nullptr};
            auto cur = head;
            auto it = other.begin();
            while (true)
            {
                cur->data = *it;

                auto tmp = it;
                ++tmp;
                if (tmp == other.end()) // 어차피 end()는 nullptr
                    break;

                cur->next = new node{0, nullptr};
                cur = cur->next;
                it = tmp;
            }
        }
    }

    // initializer_list ( 중괄호로 묶인 초기화 리스트 )를 이용한 생성자
    singly_ll(const std::initializer_list<int> &ilist) : head(nullptr)
    {
        // rbegin() = reverse begin 을 이용해서 역순으로 초기화 리스트의 값을 불러와
        // 값을 push_front 해준다
        for (auto it = std::rbegin(ilist); it != std::rend(ilist); it++)
            push_front(*it);
    }
};

int main()
{
    singly_ll sll = {1,2,3};
    sll.push_front(0);

    std::cout << "첫 번째 리스트: ";
    for(const auto& i : sll) 
        std::cout << i << ' '; // 출력 0 1 2 3
    std::cout << std::endl;

    auto sll2 = sll; // 복사 생성자 호출

    sll2.push_front(-1);
    std::cout << "첫 번째 리스트를 복사한 후, 맨 앞에 -1을 추가: ";
    for(const auto& i : sll2)
        std::cout << i << ' '; // 출력 -1 0 1 2 3
    std::cout << std::endl;

    std::cout << "깊은 복사 후 첫 번째 리스트: ";

    for(const auto& i : sll)
        std::cout << i << ' '; // 출력 0 1 2 3
    std::cout << std::endl;

    return 0;
}