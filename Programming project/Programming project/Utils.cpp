#include "Utils.h"
#include <sstream>

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
		timestr += std::to_string(time.tm_hour);
		timestr += ":";
		timestr += std::to_string(time.tm_min);

		datestr += std::to_string(time.tm_mday);
		datestr += "/";
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


}