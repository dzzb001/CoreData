// 2009-02-04
// XStrUtil.h
// guoshanhe
// 包装一些方便字符串操作的函数

#pragma once

#ifndef _X_STR_UTIL_H_
#define _X_STR_UTIL_H_

#include "XDefine.h"

namespace xbase {

////////////////////////////////////////////////////////////////////////////////
// XStrUtil
////////////////////////////////////////////////////////////////////////////////
class XStrUtil
{
public:
	// 去除字符串头(或尾)中在字符集中指定的字符
	static std::string& ChopHead(std::string &strSrc, const char *pcszCharSet = " \t\r\n");
	static std::string& ChopTail(std::string &strSrc, const char *pcszCharSet = " \t\r\n");
	static std::string& Chop(std::string &strSrc, const char *pcszCharSet = " \t\r\n");

	// 字符串转大写(或小写)
	static void ToUpper(char *pszSrc);
	static void ToLower(char *pszSrc);
	static void ToUpper(std::string &strSrc);
	static void ToLower(std::string &strSrc);

	// 区分大小写比较
	static int Compare(const char* pszSrc1, const char* pszSrc2, int length = -1);
	static int Compare(const std::string &str1, const std::string &str2, int length = -1);

	// 不区分大小写比较
	static int CompareNoCase(const char* pszSrc1, const char* pszSrc2, int length = -1);
	static int CompareNoCase(const std::string &str1, const std::string &str2, int length = -1);

	// 根据字符集中指定的分隔字符分解源字符串,并放置到std::vector中
	// nMaxCount指定期望得到的行数,解析到maxCount将终止并返回,不会继续解析;设为-1表示解析所有
	static uint32 Split(const std::string &strSrc, std::vector<std::string> &vItems, const char *pcszCharSet = " \r\n\t", int nMaxCount = -1);

	// 字符串转整数
	static BOOL     ToInt(const std::string &strSrc, int &nValue, int radix = 10);
	static int      ToIntDefault(const std::string &strSrc, int def = -1, int radix = 10);
	static int      TryToIntDefault(const std::string &strSrc, int def = -1, int radix = 10);
	static BOOL     ToUInt(const std::string &strSrc, uint32 &uValue, int radix = 10);
	static uint32   ToUIntDefault(const std::string &strSrc, uint32 def = 0, int radix = 10);
	static uint32   TryToUIntDefault(const std::string &strSrc, uint32 def = 0, int radix = 10);

	// 字符串转浮点型数
	static BOOL		ToFloat(const std::string &strSrc, double &value);
	static double	ToFloatDefault(const std::string &strSrc, double def = 0.0);
	static double	TryToFloatDefault(const std::string &strSrc, double def = 0.0);

	// 数值转字符串
	static std::string Tostring(int nVal, const char* cpszFormat=NULL/*"%d"*/);
	static std::string Tostring(uint32 uVal, const char* cpszFormat=NULL/*"%u"*/);
	static std::string Tostring(int64 nlVal, const char* cpszFormat=NULL/*"%lld"*/);
	static std::string Tostring(uint64 ulVal, const char* cpszFormat=NULL/*"%llu"*/);
	static std::string Tostring(double fVal, const char* cpszFormat=NULL/*"%f"*/);
};

} // namespace xbase

using namespace xbase;

#endif//_X_STR_UTIL_H_
