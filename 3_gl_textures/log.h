#pragma once

#include <fstream>
#include <iostream>

class Log{
public:
    Log(){
    }

    ~Log(){
        if(logFile.is_open())
        {
            logFile.close();
        }
    }

    void operator<<(const std::string& message){
        if (logFile.is_open()){
            logFile<<("Error: " + message + "\n");
        }
    }

private:
    static std::ofstream logFile;
};

std::ofstream Log::logFile{"errors.log", std::ios_base::ate | std::ios_base::out};
