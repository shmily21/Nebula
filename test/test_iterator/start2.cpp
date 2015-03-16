#include <cstdlib>
#include <cstdio>

template <class I,class T>
void func_impl(I iter,T t)
{
    T tmp;
    // function

}

template <class I>
inline
void func(I iter)
{
    func_impl(iter,*iter);
}

template<class TP,class TP2>
class compare
{
    //typedef TP value_type;
public:
     static bool is_equal(TP _val1,TP _val2);

};

template<class TP>
class compare<TP,short>
{
public:
     static bool is_equal(TP _val1,TP _val2);

};

template<class TP,class TP2>
bool compare<TP,TP2>::is_equal(TP _val1,TP _val2)
{
    std::fprintf(stderr, "%s\n","template");
    return true;
}


template<>
bool compare<int,int>::is_equal(int _val1,int _val2)
{
    std::fprintf(stderr,"%s\n","special");
    return true;
}

template<class TP>
bool compare<TP,short>::is_equal(TP _val1,TP _val2)
{
    std::fprintf(stderr, "%s\n","partial");
    return  true;
}

class _IC
{
public:
    _IC(void){};
    virtual ~_IC(void){};
};

int main(int argc,char* argv[])
{
    _IC A,B;
    int a(0),b(0);
    float fa(0.0),fb(0.0);
    double da(0.0),db(0.0);
    int32_t ia(0),ib(0);
    short sa(0),sb(0);

    compare<_IC,_IC>::is_equal(A,B); //template
    compare<int,int>::is_equal(a,b); //special
    compare<float,float>::is_equal(fa,fb);
    compare<double,double>::is_equal(da,db);
    //compare<int32_t>::is_equal(ia,ib);
    compare<short,short>::is_equal(sa,sb);
    //compare::is_equal()

    getchar();

}


