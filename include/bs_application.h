///////////////////////////////////////////////////////////
//  bs_application.h
//  Implementation of the Class application
//  Created on:      08-6月-2013 13:22:22
//  Original author: kunyang
///////////////////////////////////////////////////////////

#ifndef BS_APPLICATION_H
#define BS_APPLICATION_H

#include "bs_utils.h"
#include "tools/bs_string.h"
#include "bs_object.h"

typedef enum
{
    App_Module = 1,
    App_Gui = 0x1000,
    App_Console = 0x2000
}eAppTypes;
#define app() bs_application::instance ()

namespace impl {
struct application;
}
class bs_application : public bs_object, public bs_singleton<bs_application>
{
public:
    bs_application();
    virtual ~bs_application();

    static void enter(int argc, char **argv);
    //!
    //! \brief exit 退出程序
    //! \param code 退出返回代码
    //!
    static void exit(_in_ int code = 0);

    //!
    //! \brief quit 安全退出
    //!
    void quit();
    //!
    //! \brief exec 执行应用程序
    //! \param 指定程序运行与控制台、应用程序、启动模块
    //! \return
    //!
    int exec(int = App_Gui | App_Module)_fix_;

    //!
    //! \brief parameters 返回所有程序的参数(未经处理)
    //! \return
    //!
    bs_string_list parameters();
    //!
    //! \brief parameter 根据key返回一个参数
    //! \param key
    //! \return
    //!
    bs_string parameter (_in_ bs_string &key);

    // 组织信息
    //!
    //! \brief org_name 返回组织的名称
    //! \return
    //!
    bs_string org_name() _fix_;
    //!
    //! \brief org_info 返回组织的相关信息
    //! \return
    //!
    bs_string org_info() _fix_;
    //!
    //! \brief org_name 设置组织名称
    //! \param on
    //!
    void org_name (_in_ bs_string &on);
    //!
    //! \brief org_info 设置组织信息
    //! \param oi
    //!
    void org_info (_in_ bs_string &oi);

    // 程序信息
    //!
    //! \brief app_name 返回程序的名称
    //! \return
    //!
    bs_string app_name() _fix_;
    //!
    //! \brief app_version 返回程序的版本
    //! \return
    //!
    bs_string app_version() _fix_;
    //!
    //! \brief app_path 返回程序所在的相对路径
    //! \return
    //!
    bs_string app_path() _fix_;
    //!
    //! \brief app_paths  返回程序所在的绝对路径
    //! \return
    //!
    bs_string app_paths() _fix_;
    //!
    //! \brief pid 返回程序的进程id
    //! \return
    //!
    uint pid() _fix_;

    template <typename T>
    void tregister();
    template <typename T>
    void tunregister();

private:
    impl::application * impl;
};

#endif
