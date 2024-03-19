#ifndef __TRAINER__
#include <iostream>
#include <cassert>
#endif

struct Node
{
    explicit Node(int x)
            : value(x){};

    Node * next = nullptr;
    int value = 0;
};

class CQueue {
public:
    // konstruktory, destruktory
    CQueue() = default;
    CQueue(const CQueue & q)
    {
        Node *ptr;
        first = nullptr;
        last = nullptr;
        count = 0;
        ptr = q.first;
        while(ptr)
        {
            push(ptr->value);
            ptr = ptr->next;
        }
    }
    CQueue operator= (const CQueue & orig)
    {
        if (this == &orig) return *this;

        CQueue copy(orig);
        std::swap(first, copy.first);
        std::swap(last, copy.last);
        std::swap(count, copy.count);
        return *this;
    }
    ~CQueue()
    {
        Node * cur = first;
        Node * tmp;
        while(cur)
        {
            tmp = cur;
            cur = cur->next;
            delete tmp;
        }
    };

    void push(int x)
    {
        Node * newNode = new Node(x);
        if (!first)
        {
            first = newNode;
            last = newNode;
        }
        else
        {
            last->next = newNode;
            last = newNode;
        }
        count++;
    };

    int pop()
    {
        Node * popNode = first;
        first = first->next;
        int toReturn = popNode->value;
        delete popNode;
        count--;
        return toReturn;
    };

    bool isEmpty() const
    {
        bool toReturn = (first == nullptr);
        return toReturn;
    };

    size_t size() const
    {
        return count;
    };

    const int & front() const
    {
        return first->value;
    };

    const int & back() const
    {
        return last->value;
    };

private:
    Node * first = nullptr;
    Node * last = nullptr;
    int count = 0;
};

#ifndef __TRAINER__
int main() {
    CQueue queue;
    assert(queue.isEmpty());
    assert(queue.size() == 0);

    queue.push(1);
    assert(queue.size() == 1);

    queue.push(2);
    queue.push(3);
    assert(!queue.isEmpty());
    assert(queue.size() == 3);
    assert(queue.front() == 1);
    assert(queue.back() == 3);

    assert(queue.pop() == 1);
    assert(queue.size() == 2);
    assert(queue.front() == 2);
    assert(queue.back() == 3);

    queue.push(4);
    assert(queue.back() == 4);

    assert(queue.pop() == 2);
    assert(queue.pop() == 3);
    assert(queue.pop() == 4);
    assert(queue.isEmpty());
    assert(queue.size() == 0);

    // kopirujici konstruktor
    CQueue a;
    a.push(10);
    a.push(20);
    CQueue b = a;
    b.push(30);
    assert(a.size() == 2);
    assert(b.size() == 3);

    assert(a.pop() == 10);
    assert(a.size() == 1);
    assert(b.size() == 3);
    assert(!a.isEmpty());
    assert(!b.isEmpty());

    assert(a.pop() == 20);
    assert(a.isEmpty());

    assert(b.pop() == 10);

    // op=
    CQueue c, d;
    c.push(100);
    d.push(200);
    d.push(300);

    c = d;
    assert(c.pop() == 200);
    assert(c.size() == 1);
    assert(d.size() == 2);

    c = c;
    assert(c.size() == 1);
    return 0;
}
#endif
