#include "commonhelp.h"

CommonHelp::CommonHelp()
{

}

/**
 * @brief 打开注册表，读取Key对应value
 * @param path
 * @param key
 * @param value
 * @return
 */
int CommonHelp::ReadReg(const char *path, const char *key, unsigned char *value)
{
    HKEY hKey;
    int ret = ::RegOpenKeyExA(HKEY_CURRENT_USER, path, 0, KEY_READ, &hKey);
    if (ret != ERROR_SUCCESS)
    {
        qDebug() << "打开注册表失败";
        return 1;
    }
    // 读取KEY
    DWORD dwType = REG_SZ; // 数据类型
    DWORD cbData = 1024;
    ret = ::RegQueryValueExA(hKey, key, nullptr, &dwType, value, &cbData);
    // 判断读取结果
    if (ret != ERROR_SUCCESS)
    {
        qDebug() << "读取注册表 KEY 失败:" << ret;
        RegCloseKey(hKey);
        return 1;
    }
    RegCloseKey(hKey);

    return 0;
}

/**
 * @brief 写注册表 如不存在自动创建
 * @param path
 * @param key
 * @param value
 * @return
 */
int CommonHelp::WriteReg(const char *path, const char *key, const char *value)
{
    HKEY hKey;
    DWORD dwDisp;
    DWORD dwType = REG_SZ; //数据类型
    qDebug() << "写注册表:" << path << key << value;
    // 打开注册表项
    int ret = ::RegOpenKeyExA(HKEY_CURRENT_USER, path, 0, KEY_ALL_ACCESS, &hKey);
    if(ret != ERROR_SUCCESS)
    {
        qDebug() << "打开注册表项失败" << ret;
        RegCloseKey(hKey);
        return 1;
    }
    // 创建项
    ret = ::RegCreateKeyExA(HKEY_CURRENT_USER, path, 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr, &hKey, &dwDisp);
    if (ret != ERROR_SUCCESS)
    {
        qDebug() << "创建注册表失败";
        RegCloseKey(hKey);
        return 1;
    }
    // 设置键值
    ret = RegSetValueExA(hKey, key, 0, dwType, (BYTE*)value, strlen(value));
    if (ret != ERROR_SUCCESS)
    {
        qDebug() << "注册表中创建KEY VALUE失败";
        RegCloseKey(hKey);
        return 1;
    }
    qDebug() << "创建注册表成功";
    RegCloseKey(hKey);
    return 0;
}

/**
 * @brief 删除注册表
 * @param path
 * @return
 */
int CommonHelp::DelRegKey(const char *path, const char *key)
{
    HKEY hKey;
    // 打开注册表项
    int ret = ::RegOpenKeyExA(HKEY_CURRENT_USER, path, 0, KEY_ALL_ACCESS, &hKey);
    if(ret != ERROR_SUCCESS)
    {
        qDebug() << "打开注册表项失败" << ret;
        return 1;
    }
    // 删除键值
    ret = RegDeleteValueA(hKey, key);
    if (ret == ERROR_SUCCESS)
    {
        qDebug() << "删除注册表成功";
        return 0;
    }
    else
    {
        qDebug() << "删除注册表失败" << ret;
        return 1;
    }
}
