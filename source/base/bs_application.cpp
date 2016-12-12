#include "bs_application.h"
#include "bs_filesystem.h"
#include "bs_platform.h"

namespace impl {
struct application
{
    bs_string      app_path;
    bs_string      app_name;
    bs_string_list app_parame;

    bool           abort;
    int            abort_code;

    bs_map<bs_string, void*> app_type;
};
}
template <typename T>
void bs_application::tregister()
{
    bs_string k = bs_typeinfo<T>();
    if (!impl->app_type.contains (k))
        impl->app_type.append (k,bsNew(T));
}
template <typename T>
void bs_application::tunregister()
{
    bs_string k = bs_typeinfo<T>();
    bsDelete (impl->app_type.find (k).value ());
    impl->app_type.remove (k);
}
bs_application::bs_application():
    bs_object()
{
    impl = (bsNew (impl::application()));
    impl->app_path = bs_filesystem::current();
    impl->app_name = bs_string();
    impl->app_parame.clear();
}

bs_application::~bs_application()
{
    bsDelete (impl);
}

void bs_application::enter(int argc, char **argv)
{
    if (argc < 1)
        return ;

    app()->impl->app_parame.clear();

    for (int i= 1; i < argc; ++i)
        app()->impl->app_parame.append(argv[i]);

    bs_fileinfo finfo(argv[0]);
    app()->impl->app_name = finfo.name ();
    app()->impl->app_path = finfo.drive () + finfo.path ();
}


void bs_application::exit(_in_ int code)
{
    app()->impl->abort_code = code;
    app()->impl->abort = true;
}
void bs_application::quit()
{
    app()->impl->abort = true;
}

bs_string bs_application::app_path()_fix_
{
    if (bs_filesystem::is_directory(impl->app_path))
        return impl->app_path;
    else
        return bs_filesystem::current();
}
bs_string bs_application::app_paths()_fix_
{
    if (bs_filesystem::is_directory(impl->app_path))
        return impl->app_path;
    else
        return bs_filesystem::current();
}
bs_string bs_application::app_name()_fix_
{
    return impl->app_name;
}


static void signal_handler(int s)
{
#if bsPlatform == bsPlatform_Win32
    const int SC = SIGBREAK;
#else
    const int SC = SIGKILL;
#endif
    switch (s)
    {
#if bsPlatform != bsPlatform_Win32
        case SIGTSTP:
            raise(SC);
            break;
#endif
        case SIGINT:
        {
            //piplatform ppm = app()->platform();
            //piwindow pwm = app()->window();
            //if (pwm.get() && ppm.get())
            //    app()->exit();
            //else
                raise(SC);
        } break;
        case SIGSEGV:
        log_error("Program Access Memory Overflow (SIGSEGV)");
#if bsPlatform != bsPlatform_Win32
            raise(SC);
#endif
            break;
        case SIGABRT:
            /*if (!qAppOK)
            {
                canDeleteApp = false;
                longjmp(env, 1);
            }*/
            log_error("Interrupt program run(SIGABRT)");
#if bsPlatform != bsPlatform_Win32
            raise(SC);
#endif
            break;
        case SIGFPE:
            log_error("Program Access Memory Overflow (SIGFPE)");
#if bsPlatform != bsPlatform_Win32
            raise(SC);
#endif
            break;
    }
}
int bs_application::exec(int at)_fix_
{
    int ret = 0;
    signal(SIGSEGV, signal_handler);
    signal(SIGINT, signal_handler);
#if bsPlatform != bsPlatform_Win32
    signal(SIGTSTP, signal_handler);
#endif
    signal(SIGFPE, signal_handler);
    signal(SIGSEGV, signal_handler);
    signal(SIGABRT, signal_handler);

    if (at & App_Console)
    {
        while (!impl->abort)
            bs_thread::yield();
    }
    else
        ret = bs_platform::instance ()->dispatch ();

    return ret;
}
