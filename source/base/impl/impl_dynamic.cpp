#include "impl_dynamic.h"
#include "bs_filesystem.h"

impl_dynamic::impl_dynamic()
{
    _hinst = NULL;
    this->io_eof = false;
    this->io_error = Io_NotError;
    this->io_open = false;
    this->io_operation_mode = Io_Not;
    this->io_pos = 0;
    this->io_size = 0;
}
impl_dynamic::~impl_dynamic()
{
    if (is_open ())
        close();
    this->io_eof = false;
    this->io_error = Io_NotError;
    this->io_open = false;
    this->io_operation_mode = Io_Not;
    this->io_pos = 0;
    this->io_size = 0;
}
bs_string impl_dynamic::suffix()const
{
#if bsPlatform == bsPlatform_Win32
        return bs_string(".dll");
#elif bsPlatform == bsPlatform_Apple || bsPlatform == bsPlatform_Ios
        return bs_string(".dylib");
#else
        return bs_string(".so");
#endif
}

bs_string impl_dynamic::errors()const
{
    return bs_string();
}

bool impl_dynamic::open(int)
{
    if (path().is_empty() || is_open () || _hinst)
        return false;
    bs_string tpath;
    if (io_url.protocol () == Protocol_File)
        tpath = bs_filesystem::slash (io_url.url ());
    else
        return false;
#if bsPlatform == bsPlatform_Win32
    _hinst = LoadLibraryA((char*)tpath.to_utf8().data());

    if (_hinst == NULL)
    {
        io_error = Io_OpenError;
        log_warnings("dynamic load error(code=%d, %s)",
                     GetLastError(), (char*)path().to_utf8().data());
        return false;
    }
    return true;
#elif  bsPlatform == bsPlatform_Apple || bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Linux ||\
    bsPlatform == bsPlatform_Android || bsPlatform == bsPlatform_Ios
    _hinst = dlopen((char*)tpath.to_latin1(), RTLD_NOW | RTLD_LOCAL);

    if (_hinst == NULL)
    {
        io_error = Io_OpenError;
        log_warnings("dynamic load error(code=%s, %s)", dlerror(), path().to_latin1());
        return false;
    }
    return true;
#endif
}
void impl_dynamic::close()
{
    if (_hinst != NULL)
    {
#if bsPlatform == bsPlatform_Win32
        FreeLibrary((HINSTANCE) _hinst);
#elif bsPlatform == bsPlatform_Apple || bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Linux || \
    bsPlatform == bsPlatform_Android || bsPlatform == bsPlatform_Ios
        dlclose(_hinst);
#else

#endif
        _hinst = NULL;
    }
    this->io_eof = false;
    this->io_error = Io_NotError;
    this->io_open = false;
    this->io_operation_mode = Io_Not;
    this->io_pos = 0;
    this->io_size = 0;
}
void *impl_dynamic::symbol(const bs_string &name)
{
    if (name.is_empty() || !is_open () || !_hinst)
        return NULL;
#if bsPlatform == bsPlatform_Win32
    void* procAddr = (void*)GetProcAddress((HINSTANCE) _hinst,
                                           (char*)name.to_utf8().data());

    if (procAddr == NULL)
    {
        //char* lpMsg = formatMessage(GetLastError());

        //aprintf("Unable to find symbol [%s]: %s\n", name, lpMsg);
        //LocalFree(lpMsg);
    }

    return procAddr;
#elif  bsPlatform == bsPlatform_Apple || bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Linux ||\
    bsPlatform == bsPlatform_Android || bsPlatform == bsPlatform_Ios
    void* procAddr = dlsym(_hinst, name.to_latin1());

    if (procAddr == NULL)
        log_warnings("dynamic symbol error(name=%s, %s)", name.to_latin1(), dlerror());

    return procAddr;
#endif
}
