#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

#define maxLines 28

std::atomic<bool> keepRunning{true};
std::vector<std::string> dates(maxLines, "");
std::mutex datesMutex;

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

struct Date {
    int year, month, day;
};

std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);

    std::ostringstream oss;
    oss << (now_tm->tm_year + 1900) << '-' 
        << std::setw(2) << std::setfill('0') << (now_tm->tm_mon + 1) << '-'
        << std::setw(2) << std::setfill('0') << now_tm->tm_mday;
    return oss.str();
}

Date parseDate(const std::string& dateStr) {
    Date date;
    char delimiter;
    std::istringstream iss(dateStr);
    iss >> date.year >> delimiter >> date.month >> delimiter >> date.day;
    return date;
}

std::chrono::system_clock::time_point toTimePoint(const Date& date) {
    std::tm tm = {};
    tm.tm_year = date.year - 1900;
    tm.tm_mon = date.month - 1;
    tm.tm_mday = date.day;
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

void cout_function() {
    int dates_count[maxLines] = {0};
    int prev_dates_count[maxLines] = {0};
    std::cout << "****************************************************************************************************************" << std::endl;
    std::cout << "*                                  |*|                                   |*|                                   *" << std::endl;
    std::cout << "*" 
                << " 3a : " << std::setw(3) << dates_count[6] << " | "
                << " 3b : " << std::setw(3) << dates_count[7] << " | "
                << " 3c : " << std::setw(3) << dates_count[8] << " |*| "
                << " 6a : " << std::setw(3) << dates_count[15] << " | "
                << " 6b : " << std::setw(3) << dates_count[16] << " | "
                << " 6c : " << std::setw(3) << dates_count[17] << " |*| "
                << " 9a : " << std::setw(3) << dates_count[24] << " | "
                << " 9b : " << std::setw(3) << dates_count[25] << " | "
                << " 9c : " << std::setw(3) << dates_count[26] << " *"
                << std::endl
                << "*                                  |*|                                   |*|                                   *" << std::endl
                << "*" 
                << " 2a : " << std::setw(3) << dates_count[3] << " | "
                << " 2b : " << std::setw(3) << dates_count[4] << " | "
                << " 2c : " << std::setw(3) << dates_count[5] << " |*| "
                << " 5a : " << std::setw(3) << dates_count[12] << " | "
                << " 5b : " << std::setw(3) << dates_count[13] << " | "
                << " 5c : " << std::setw(3) << dates_count[14] << " |*| "
                << " 8a : " << std::setw(3) << dates_count[21] << " | "
                << " 8b : " << std::setw(3) << dates_count[22] << " | "
                << " 8c : " << std::setw(3) << dates_count[23] << " *"
                << std::endl
                << "*                                  |*|                                   |*|                                   *" << std::endl
                << "*" 
                << " 1a : " << std::setw(3) << dates_count[0] << " | "
                << " 1b : " << std::setw(3) << dates_count[1] << " | "
                << " 1c : " << std::setw(3) << dates_count[2] << " |*| "
                << " 4a : " << std::setw(3) << dates_count[9] << " | "
                << " 4b : " << std::setw(3) << dates_count[10] << " | "
                << " 4c : " << std::setw(3) << dates_count[11] << " |*| "
                << " 7a : " << std::setw(3) << dates_count[18] << " | "
                << " 7b : " << std::setw(3) << dates_count[19] << " | "
                << " 7c : " << std::setw(3) << dates_count[20] << " *"
                << std::endl;
    std::cout << "*                                  |*|                                   |*|                                   *" << std::endl;
    std::cout << "****************************************************************************************************************" << std::endl;
    std::cout << "Enter a section number (1a - 9c) to store or reset the date (0 to quit): "<<std::endl;

    while (keepRunning) {
        std::string currentDate = getCurrentDate();
        bool hasChanged = false;

        {
            std::lock_guard<std::mutex> lock(datesMutex);
            for(int i = 0; i < maxLines; i++) {
                if (!dates[i].empty()) {
                    Date savedDate = parseDate(dates[i]);
                    Date current = parseDate(currentDate);

                    auto savedTimePoint = toTimePoint(savedDate);
                    auto currentTimePoint = toTimePoint(current);

                    using Days = std::chrono::duration<int, std::ratio<86400>>;
                    auto duration = std::chrono::duration_cast<Days>(currentTimePoint - savedTimePoint);

                    dates_count[i] = duration.count() + 1;
                }
                else {
                    dates_count[i] = 0;
                }
                if (dates_count[i] != prev_dates_count[i]) {
                    hasChanged = true;
                }
            }
            if(hasChanged) {
                std::copy(std::begin(dates_count), std::end(dates_count), std::begin(prev_dates_count));
            }
        }

        if (hasChanged) {
            clearScreen();
            std::cout << "****************************************************************************************************************" << std::endl;
            std::cout << "*                                  |*|                                   |*|                                   *" << std::endl;
            std::cout << "*" 
                      << " 3a : " << std::setw(3) << dates_count[6] << " | "
                      << " 3b : " << std::setw(3) << dates_count[7] << " | "
                      << " 3c : " << std::setw(3) << dates_count[8] << " |*| "
                      << " 6a : " << std::setw(3) << dates_count[15] << " | "
                      << " 6b : " << std::setw(3) << dates_count[16] << " | "
                      << " 6c : " << std::setw(3) << dates_count[17] << " |*| "
                      << " 9a : " << std::setw(3) << dates_count[24] << " | "
                      << " 9b : " << std::setw(3) << dates_count[25] << " | "
                      << " 9c : " << std::setw(3) << dates_count[26] << " *"
                      << std::endl
                      << "*                                  |*|                                   |*|                                   *" << std::endl
                      << "*" 
                      << " 2a : " << std::setw(3) << dates_count[3] << " | "
                      << " 2b : " << std::setw(3) << dates_count[4] << " | "
                      << " 2c : " << std::setw(3) << dates_count[5] << " |*| "
                      << " 5a : " << std::setw(3) << dates_count[12] << " | "
                      << " 5b : " << std::setw(3) << dates_count[13] << " | "
                      << " 5c : " << std::setw(3) << dates_count[14] << " |*| "
                      << " 8a : " << std::setw(3) << dates_count[21] << " | "
                      << " 8b : " << std::setw(3) << dates_count[22] << " | "
                      << " 8c : " << std::setw(3) << dates_count[23] << " *"
                      << std::endl
                      << "*                                  |*|                                   |*|                                   *" << std::endl
                      << "*" 
                      << " 1a : " << std::setw(3) << dates_count[0] << " | "
                      << " 1b : " << std::setw(3) << dates_count[1] << " | "
                      << " 1c : " << std::setw(3) << dates_count[2] << " |*| "
                      << " 4a : " << std::setw(3) << dates_count[9] << " | "
                      << " 4b : " << std::setw(3) << dates_count[10] << " | "
                      << " 4c : " << std::setw(3) << dates_count[11] << " |*| "
                      << " 7a : " << std::setw(3) << dates_count[18] << " | "
                      << " 7b : " << std::setw(3) << dates_count[19] << " | "
                      << " 7c : " << std::setw(3) << dates_count[20] << " *"
                      << std::endl;
            std::cout << "*                                  |*|                                   |*|                                   *" << std::endl;
            std::cout << "****************************************************************************************************************" << std::endl;
            std::cout << "Enter a section number (1a - 9c) to store or reset the date (0 to quit): "<<std::endl;
        }
    }
}

int main() {
    std::string filename = "date.txt";
    std::string input;

    std::ifstream inFile(filename);
    std::thread coutThread(cout_function);

    while(keepRunning) {
        if(inFile.is_open()){
            std::lock_guard<std::mutex> lock(datesMutex);
            std::string line;
            int lineIndex = 0;
            while(std::getline(inFile, line) && lineIndex < maxLines) {
                dates[lineIndex] = line;
                lineIndex++;
            }
            inFile.close();
        }

        int lineNum = 0;
        std::cin >> input;
        if(input != "0"){
            for(char ch: input){
                if(std::isdigit(ch)){
                    lineNum = (ch - '0' - 1) * 3;
                }
                else if(std::isalpha(ch)){
                    if(ch == 'a'){
                        lineNum += 1;
                    }
                    else if(ch == 'b'){
                        lineNum +=2;
                    }
                    else if(ch == 'c'){
                        lineNum +=3;
                    }
                }
            }
        }
        else{
            lineNum = 0;
        }

        if(lineNum >= 1 && lineNum <= maxLines) {
            int index = lineNum - 1;

            {
                std::lock_guard<std::mutex> lock(datesMutex);
                if(!dates[index].empty()) {
                    dates[index] = "";
                }
                else{
                    std::string currentDate = getCurrentDate();
                    dates[index] = currentDate;
                }
            }
        }
        else if(lineNum == 0) {
            std::cout << "Program terminated by user.\n";
            keepRunning = false;
        }
        else{
            std::cerr << "Invalid input. Please enter a number between 1a and 9c.\n";
        }

        {
            std::lock_guard<std::mutex> lock(datesMutex);
            std::ofstream outFile(filename);
            if(outFile.is_open()) {
                for(const auto& date : dates) {
                    outFile << date << "\n";
                }
                outFile.close();
                std::cout << "Date updated successfully.\n";
            }
            else {
                std::cerr << "Unable to open file for writing.\n";
            }
        }
    }

    coutThread.join();

    return 0;
}