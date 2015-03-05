#include <cstdlib>
#include <cstdio>

size_t ROUND_UP(size_t __bytes)
{
    return ((__bytes)+7) & ~(7);
}


int main(int argc,char* argv[])
{
    std::fprintf(stderr,"18=> %d\n",ROUND_UP(18));
    int count(1280);
    while(count<=1280 && count>=0)
    {

        int __bytes=count;
        if(count%8==0)
        std::fprintf(stderr,"2*%d+%d=%d\n",__bytes,ROUND_UP(__bytes>>4),2*__bytes+ROUND_UP(__bytes>>4));
        --count;
    }
    //std::fprintf(stderr,"%d\n",count%8);
}
