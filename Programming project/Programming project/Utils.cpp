#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "Utils.h"
#include <sstream>
#include <locale>
#include <codecvt>

namespace Utils {

	int toInt(char c) {
		return c - '0';
	}


	std::string toUpper(std::string str) {
		for (char& ch : str) ch=std::toupper(ch);
		return str;
	}
	std::string toLower(std::string str) {
		for (char& ch : str) ch=std::tolower(ch);
		return str;
	}
	SDL_Rect getWindowBounds(SDL_Window* window) {
		int w;
		int h;
		SDL_GetWindowSize(window, &w, &h);
		return  { 0,0,w,h };
	}
	Vector2 getTextureDims(SDL_Texture* t) {
		int w;
		int h;
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		return Vector2( w,h );
	}

	std::vector<std::string> split(std::string str,char delimiter) {
		std::vector<std::string> v = {};
		std::stringstream ss(str);
		std::string buffer;
		while (std::getline(ss, buffer, delimiter)) v.push_back(buffer);
		return v;
	}
	
	Utils::DateAndTime timeToDateTime(std::tm time) {
		std::string timestr = "";
		std::string datestr = "";
		//pad time and date with zeros
		if (time.tm_hour < 10) timestr += "0";
		timestr += std::to_string(time.tm_hour);
		timestr += ":";
		if (time.tm_min < 10) timestr += "0";
		timestr += std::to_string(time.tm_min);

		if (time.tm_mday < 10) datestr += "0";
		datestr += std::to_string(time.tm_mday);
		datestr += "/";
		if (time.tm_mon+1 < 10) datestr += "0";
		datestr += std::to_string(time.tm_mon+1);
		datestr += "/";
		datestr += std::to_string(time.tm_year+1900);
		return { timestr,datestr };
	}

	std::tm DateTimeToTm(Utils::DateAndTime time) {
		std::tm tm = {};
		std::vector<std::string> timesplit=Utils::split(time.time, ':');
		tm.tm_hour = std::stoi(timesplit[0]);
		tm.tm_min = std::stoi(timesplit[1]);

		std::vector<std::string> datesplit = Utils::split(time.date, '/');
		tm.tm_mday = std::stoi(datesplit[0]);
		tm.tm_mon = std::stoi(datesplit[1])-1;
		tm.tm_year = std::stoi(datesplit[2])-1900;
		return tm;
	}

	DateAndTime getCurrentDateAndTime() {
		std::time_t currentTime = std::time(nullptr);
		std::tm* localTime = std::localtime(&currentTime);
		return Utils::timeToDateTime(*localTime);
	}

	std::wstring stringTowstring(std::string str) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
		std::wstring wstr = converter.from_bytes(str);
		return wstr;
	}

	std::string eraseCharFromString(std::string str, char ch) {
		str.erase(std::remove_if(str.begin(), str.end(), [ch](char c) { return c == ch; }), str.end());
		return str;
	}

	SDL_Color setRenderDrawColor(SDL_Renderer* renderer, SDL_Color color) {
		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(renderer,&r, &g, &b, &a);
		SDL_Color c = {r, g, b, a};
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		return c;
	}

}


