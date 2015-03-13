#include <cstdlib>
#include <cstdio>
#include <iostream>

template<class T>
class nebula_ptr
{
public:
    explicit nebula_ptr(T *p=0):pointee(p)
    {

    }

    template<class U>
    nebula_ptr(nebula_ptr<U>& rhs ):pointee(rhs.release())
    {

    }

    ~nebula_ptr() {delete pointee;};

    template<class U>
    nebula_ptr<U>& operator=(nebula_ptr<U>& rhs)
    {
        if(this!=&rhs) reset(rhs.release());
        return *this;
    }

    T& operator*() const
    { return *pointee;};

    T* operator->() const
    { return pointee;}

    T* get() const
    { return pointee; }

private:
    T *pointee;
};

template <typename T>
class ListItem
{
public:
    T value() const { return _value;};
    ListItem* next() const {return _next;};
private:
    T               _value;
    ListItem*       _next;
};

template<typename T>
class List
{
public:
    void insert_front(T value);
    void insert_end(T value);
    void display(std::ostream &os=std::cout) const;
private:
    ListItem<T>* _end;
    ListItem<T>* _front;
    long _size;
};



template <class Item>
struct ListIter
{
    Item* ptr;
    ListIter(Item* p=0):ptr(p)
    {

    }

    Item& operator*() const
    {
        return *ptr;
    };

    Item* operator->() const
    {
        return ptr;
    };

    ListIter& operator++()
    {
        ptr = ptr ->next();
        return  *this;
    }

    ListIter& operator++(int)
    {
        ListIter tmp = *this;
        ++*this;
        return tmp;
    }

    bool operator==(const ListIter& i) const
    {
        return ptr==i.ptr;
    }

    bool operator!=(const ListIter& i) const
    {
        return ptr != i.ptr;
    }

};


int main(int agrc,char* argv[])
{
    List<int> mylist;
    for(int i=0;i<5;++i)
    {
        mylist.insert_front(i);
        mylist.insert_end(i+2);
    }
    mylist.display();

    ListIter<ListItem<int> > begin(mylist.front());
    ListIter<ListItem<int> > end;
    ListIter<ListItem<int> > iter;

    iter=std::find(begin,end(),3);
    if(iter==end)
    {
        std::cout<<"not found"<<std::endl;
    }
    else
    {
        std::cout<<"found"<<iter->value()<<std::endl;
    }

    iter=std::find(beign,end,7);
    if(iter==end)
    {
        std::cout<<"not found"<<iter->value()<<std::endl;
    }
    else
    {
        std::cout<<"found"<<iter->value()<<std::endl;
    }
}