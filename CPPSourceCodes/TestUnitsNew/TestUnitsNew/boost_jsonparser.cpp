#include "stdafx.h"
#include "Work.h"

//#include "utf8.h"

#include <boost/locale.hpp>
#include <codecvt>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

void WorkImpl<boost_jsonparser>::DoWork()
{
	boost::property_tree::ptree pt;	
	//pt.push_back( boost::property_tree::json_parser::value_type(  ) );
	//pt.put("wow", 32);
	//std::wstring_convert< std::codecvt_utf8_utf16<char32_t>, char16_t > conversion;
	//std::string mbs(conversion.to_bytes( std::u16string(L"인증오류로 인해.") ));
	//std::string strU8str( u8"인증오류로 인해.");
	std::string normalStr("인증오류로 인해.");
	//특정필드를 UTF8로 만드는 코드
	std::string utf8_string1( boost::locale::conv::to_utf<char>( normalStr, "EUC-KR" ) );


	pt.put<int>("wow", 32);

	char szWord[] = {"인증오류로 인해."};
	pt.put("woo-ah", szWord);
	//pt.put("woo-ah", normalStr);
	//pt.put("woo-ah", utf8_string1);
	//pt.push_back( boost::property_tree::value_type( "woo-ah", utf8_string1 ) );

	//std::locale loc("ko_kr.UTF8");

	std::stringstream ssReq;
	boost::property_tree::json_parser::write_json( ssReq, pt );

	std::string strReq(ssReq.str());
	//JSON 구문 전체를 utf8로 바꾸는 구문
	std::string utf8_string( boost::locale::conv::to_utf<char>( strReq, "EUC-KR" ) );
	std::auto_ptr<char> pszStr(new char[ strReq.length()+1]);
	size_t lenSzChar=  sizeof( (*pszStr.get()) ) * strReq.length()+1;
	strcpy( pszStr.get(), strReq.c_str() );
	std::auto_ptr<wchar_t> wszChar( new wchar_t[strReq.length()+1] );
	size_t lenWszChar= sizeof( (*wszChar.get())) * strReq.length()+1;
	MultiByteToWideChar( CP_ACP, 0, pszStr.get(), -1, wszChar.get(), strReq.length()+1 );
	
	std::auto_ptr<char> psz8Str(new char[ strReq.length()+1]);
	WideCharToMultiByte( CP_UTF8, 0, wszChar.get(), -1, psz8Str.get(), strReq.length()+1, NULL, NULL );

	boost::property_tree::ptree ptRes;

	std::stringstream ssRes(strReq);


	std::string strCvtUTF(boost::locale::conv::from_utf<char>(strReq, "EUC-KR"));

	boost::property_tree::json_parser::read_json( ssRes , ptRes );

	int nRet = ptRes.get( "wow", 0 );

	//std::wstring wstrRes(ptRes.get<std::wstring>("woo-ah", L"unknown"));
	auto it = ptRes.find("woo-ah");
	auto sec = it->second;
	std::string strRes( ptRes.get("woo-ah", "unknown") );
	std::string strMBCSRes( boost::locale::conv::from_utf<char>(strRes, "EUC-KR") );
}
