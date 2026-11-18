#include <iostream>
#include <string>
#include <filesystem>
#include <functional>
#include <map>
#include "standard.hpp"
#include <chrono>
#include <conio.h>
#include <random>
#include <thread>


float ver = 0.1f;
std::filesystem::path path = std::filesystem::current_path();
std::string spath = path.string();

std::map<std::string, std::function<std::string(std::string)>> commands;

std::string helpmsg = "Built-in Commands\nhelp - shows all commands\nexit - exits the terminal";

std::string help(std::string input)
{
    return helpmsg;
}


void addtohelp(std::string options, std::string libname)
{
    helpmsg += "\n\n" + libname + " Commands\n" + options;
}

int main()
{
    std::cout << "--- DelayKopf " + std::to_string(ver) + "\n";
    std::string input;
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> delayMilliseconds(100, 600);
    std::uniform_int_distribution<int> missedKeyChance(1, 100);
    commands["help"] = help;
    commands["exit"] = [](std::string input) {
        exit(0);
        return std::string("");
        };
    standard::registerLib(&commands, addtohelp, &path, &spath);
    while (true)
    {
        input = "";
        std::cout << spath + ">";
        while (true)
        {
            int key = _getch();

            if (key == '\r')
            {
                std::cout << '\n';
                break;
            }

            if (key == '\b')
            {
                if (!input.empty())
                {
                    input.pop_back();
                    std::cout << "\b \b";
                }
                continue;
            }

            if (key == 0 || key == 0xE0)
            {
                static_cast<void>(_getch());
                continue;
            }

            if (missedKeyChance(generator) > 25)
            {
                input += static_cast<char>(key);
                std::cout << static_cast<char>(key) << std::flush;
            }
            if (missedKeyChance(generator) > 98)
                exit(0);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }

        if (!commands.contains(input.substr(0, input.find(' '))))
            std::cout << "Unknown command \"" << input.substr(0, input.find(' ')) << "\".\n";
        else
            std::cout << commands[input.substr(0, input.find(' '))](input) << "\n";
    }
}