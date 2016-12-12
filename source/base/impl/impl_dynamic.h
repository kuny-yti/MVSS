
#ifndef IMPL_DYNAMIC_H
#define IMPL_DYNAMIC_H

#include "interface/iio.h"

#if bsPlatform == bsPlatform_Win32
#include <windows.h>
#elif bsPlatform == bsPlatform_Apple || bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Linux || \
        bsPlatform == bsPlatform_Android || bsPlatform == bsPlatform_Ios
#include <dlfcn.h>
#else

#endif

class impl_dynamic : public iio
{
    friend class bs_filesystem;
public:
    impl_dynamic();
    ~impl_dynamic();

    virtual bs_string errors()const;

    virtual bool open(int = Io_ReadWrite);
    virtual void close() ;

    bs_string suffix()const ;
    void *symbol(const bs_string &name);

private:
    virtual int64 read(void *, int64 )const{return -1;}
    virtual int64 write(void *, int64)const{return -1;}
    virtual bool seek(int64){}

    void* _hinst;
};

#endif //IMPL_DYNAMIC_H
