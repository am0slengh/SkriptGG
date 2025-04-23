#pragma once
#include <string>
#include <vector>
#include "curl/curl.h"
#include "EncryptString.hpp"
#include "EncryptFuncs.hpp"

extern bool IsValid;
static std::string APIKEY = E("6MAYS3DR4EXL"); // ENTER USER ID
static std::string userid = E("494"); // ENTER USER ID
static std::string ProgramID = E("64ckEX7TQg9N"); // ENTER PROGRAM ID
static std::string ProgramName = E("Sucka"); // ENTER PROGRAM Name
static std::string ProgramEncryption = E("JV65A3LM630TPCQ4EURSIBD15GHIRW9U"); // ENTER PROGRAM Encryption Key
static std::string Response1 = E("VL1C5J1Q");
static std::string ResponseInvalid = E("Q8MY5FHD");
static std::string ResponseExpired = E("UE5JDBR7");
static std::string ResponseHash = E("F3I0P7CM");
static std::string Response2 = E("FTEQ837M");
static std::string ResponseBanned = E("YYHTEU79");

class program
{
public:
	static std::tuple<std::string, std::string, std::string> login(std::string key, std::string userid, std::string pid, std::string programname, std::string skey);
	static std::vector<uint8_t> Stream(std::string key, std::string link);
};
static char PassWord[50] = "";


static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

static void BAN_USER(std::string KEY, std::string REASON)
{
	CURL* curl = curl_easy_init();

	std::string data;

	std::string to_return;

	std::string link = E("https://panel.proxine.ninja/api/apiaccess.php?api=") + APIKEY + E("&action=ban&program=") + ProgramName + E("&key=") + KEY + E("&reason=") + REASON;

	curl_easy_setopt(curl, CURLOPT_URL, link.c_str());

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &to_return);

	auto code = curl_easy_perform(curl);


	curl_easy_cleanup(curl);
}