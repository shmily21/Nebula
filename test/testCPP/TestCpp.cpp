#include <cstdlib>
#include <cstdio>


template<typename Ty>
class testCPP
{
public:
    static const char m_Data0=1;
};

class testCPP2
{
public:
    static const char m_Data0=2;
};

class testCPP3
{
public:
    static const char m_Data0=3;
};

class testCPP4
{
public:
    //static char m_Data0=4;
};

int main(int argc,char* argv[])
{
    union obj
    {
        union obj* free_list;
        char client_data[1];
    };

    union obj2
    {
        union obj* free_list;
    };

    union obj3
    {
        char client_data[1];
    };

    int64_t s(1000);

    std::fprintf(stderr, "1 size: %d\n",sizeof(obj));
    std::fprintf(stderr, "2 size: %d\n",sizeof(obj2));
    std::fprintf(stderr, "3 size: %d\n",sizeof(obj3));
    std::fprintf(stderr, "3 size: %d\n",sizeof(&s));
    getchar();
}