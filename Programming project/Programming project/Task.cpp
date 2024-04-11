#define _CRT_SECURE_NO_WARNINGS
#include "task_objects.h"
#include "Main_Functions.h"
#include <filesystem>
#include <chrono>
#include <vector>
#include <array>
#include <Windows.h>
#include <thread>
#include <sstream>

Task::Task(std::string taskName,
	std::string frequency,
	std::string fileName,
	std::string extraArgs,
	std::string whenToRun,
	std::string time) : taskName(taskName), time(time), frequency(frequency), fileName(fileName), extraArgs(extraArgs),whenToRun(whenToRun) {
	std::time_t currentTime = std::time(nullptr);
	std::tm* localTime = std::localtime(&currentTime);
	this->date = Utils::timeToDateTime(*localTime).date;
}

Task::Task(std::string taskName,
	std::string frequency,
	std::string fileName,
	std::string extraArgs,
	std::string whenToRun,
	std::string time,
	std::string date) : taskName(taskName), time(time), frequency(frequency), fileName(fileName), extraArgs(extraArgs), whenToRun(whenToRun), date(date) {

	}



std::vector<std::string> Task::isValid() {
	std::vector<std::string> err;
	if (this->taskName == "") err.push_back("Task Name is blank");

	if (!std::filesystem::exists(this->fileName)) {
		err.push_back("Specified file: " + this->fileName + " does not exist");
	}

	//ensure that the when to run value is valid and if set to run at a specific time ensure the time is valid
	std::array<std::string,2> whenToRunValidOpts = { "Immediately","At set time" };
	if (std::find(whenToRunValidOpts.begin(), whenToRunValidOpts.end(), this->whenToRun) != whenToRunValidOpts.end()) {
		if (this->whenToRun != "Immediately") {
			bool timeValid = true;
			if (time.size() < 4 || time.size() > 4) timeValid = false;
			else {
				//verify digits for inputted time are valid
				int nums[4];
				for (int i = 0; i < 4; i++) nums[i] = Utils::toInt(time[i]);
				if (nums[0] > 2) timeValid = false;
				else if (nums[0] == 2 && nums[1] > 4) timeValid = false;
			}
			if (!timeValid) err.push_back("Time is not valid");
		}
	}

	else {
		err.push_back("When To Run Invalid Argument");
	}

	//ensure that the frequency set is valid
	std::array<std::string, 5> frequencyVallidOpts = { "Only once","Hourly","Daily","Weekly" };
	if (std::find(frequencyVallidOpts.begin(), frequencyVallidOpts.end(), this->frequency) == frequencyVallidOpts.end()) err.push_back("Frequency Invalid Option");


	return err;
}

bool Task::execute(bool raiseErrorOnFail) {
	std::vector<std::string> errs = this->isValid();
	bool valid = (errs.size() == 0);
	if (valid) {
		if (this->whenToRun != "Immediately") {
			std::time_t time = Main::strTimeToTime(this->time);
			std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(time);
			std::this_thread::sleep_until(tp);
		}
		HINSTANCE res=ShellExecuteA(NULL, "open", this->fileName.c_str(), this->extraArgs.c_str(), NULL, SW_SHOW);
		if ((INT_PTR)res < 32 && raiseErrorOnFail) {
			std::string error=Main::getWindowsErrorMsg(GetLastError());
			Main::windowsErrMessageBoxOk("An error has occured", error);
			return false;
		}
	}
	else if (raiseErrorOnFail) {
		std::string errorStr = "An error has occured while attempting to run the file:\n";
		for (std::string taskError : errs) {
			errorStr += taskError;
			errorStr += "\n";
		}
		Main::windowsErrMessageBoxOk("An error has occured", errorStr);
		return false;
	}
	return true;

}

std::string Task::convertToExportableFormat() {
	std::stringstream ss;
	ss << this->taskName << "," << this->fileName << "," << this->extraArgs << "," << this->frequency << "," << this->whenToRun;
	if (this->whenToRun != "Immediately") ss << this->time;
	return ss.str();
}

std::time_t Task::timeAndDateTotime_t() {
	Utils::DateAndTime time = { this->time,this->date };
	std::tm tm=Utils::DateTimeToTm(time);
	return std::mktime(&tm);
}