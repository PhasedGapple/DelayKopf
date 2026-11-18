#pragma once

#include <iostream>
#include <map>
#include <functional>
#include <string>
#include <filesystem>
#include <Windows.h>

namespace standard {
	inline std::filesystem::path* path = nullptr;
	inline std::string* spath          = nullptr;

	std::string echo(std::string options)
	{
		return options.substr(options.find(' ') + 1);
	}

	std::string cd(std::string options)
	{
		std::string newpath = options.substr(options.find(' ') + 1);
		std::filesystem::current_path(*path / newpath);
		*path               = std::filesystem::current_path();
		*spath              = path->string();
		return                "";
	}

	std::string mkdir(std::string options)
	{
		std::string pathname = options.substr(options.find(' ') + 1);
		std::filesystem::create_directory(*path / pathname);
		return                 "";
	}

	std::string rm(std::string options)
	{
		std::string filename = options.substr(options.find(' ') + 1);
		std::filesystem::remove(*path / filename);
		return                 "";
	}

	std::string dir(std::string options)
	{
		std::string output;

		for (const auto& file : std::filesystem::directory_iterator(*path))
		{
			output += file.path().filename().string();
			if (file.is_directory())
				output += " <DIR>";

			auto ftime = std::filesystem::last_write_time(file);
			auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
				ftime - decltype(ftime)::clock::now() + std::chrono::system_clock::now()
			);

			std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

			char timeBuffer[26];
			ctime_s(timeBuffer, sizeof(timeBuffer), &cftime);

			output += " | ";
			output += timeBuffer;
			output += "";
		}

		return output;
	}

	std::string cp(std::string options)
	{
		size_t first = options.find(' ') + 1;
		size_t second = options.find(' ', first);

		std::string arg1 = options.substr(first, second - first);
		std::string arg2 = options.substr(second + 1);

		std::filesystem::copy_file(arg1, arg2);

		return "";
	}

	std::string mv(std::string options)
	{
		size_t first = options.find(' ') + 1;
		size_t second = options.find(' ', first);

		std::string arg1 = options.substr(first, second - first);
		std::string arg2 = options.substr(second + 1);

		std::filesystem::rename(arg1, arg2);

		return "";
	}

	std::string clear(std::string options)
	{
		system("cls");
		
		return "";
	}

	std::string ram(std::string options)
	{
		MEMORYSTATUSEX mem = {};
		mem.dwLength = sizeof(mem);

		GlobalMemoryStatusEx(&mem);

		std::string answer = "RAM usage: ";
		answer += std::to_string((mem.ullTotalPhys - mem.ullAvailPhys) / 1024 / 1024);
		answer += "MB/";
		answer += std::to_string(mem.ullTotalPhys / 1024 / 1024);
		answer += "MB";

		return answer;
	}



	void registerLib(std::map<std::string, std::function<std::string(std::string)>>* commands, void (*addHelp)(std::string, std::string), std::filesystem::path* path1, std::string* spath1)
	{
		(*commands)[std::string("echo")]  = echo;
		(*commands)[std::string("cd")]    = cd;
		(*commands)[std::string("mkdir")] = mkdir;
		(*commands)[std::string("rm")]    = rm;
		(*commands)[std::string("dir")]   = dir;
		(*commands)[std::string("ls")]    = dir;
		(*commands)[std::string("cp")]    = cp;
		(*commands)[std::string("copy")]  = cp;
		(*commands)[std::string("mv")]    = mv;
		(*commands)[std::string("move")]  = mv;
		(*commands)[std::string("clear")] = clear;
		(*commands)[std::string("cls")]   = clear;
		(*commands)[std::string("ram")]   = ram;

		path                              = path1;
		spath                             = spath1;

		addHelp("echo MSG - repeats your message\ncd PATH - changes the working directory\nmkdir NAME - creates a directory\nrm FILENAME - removes a file or directory\ndir, ls - lists files and folders in a directory\ncp, copy FILE1 FILE2 - copies a file\nmv, move FILE NEWFILE - moves a file to another directory\nclear, cls - Clears the terminal window.", "Standard");
	}
}