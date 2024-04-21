// Client1S.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include "winsock2.h"
#include <string>
#include <fstream>
#include <chrono>

#pragma warning(disable: 4996)

SOCKET Connection;
char clientId = 'U';

std::ofstream logFile("D:\\Client.txt", std::ios::app);

int ProcessF(int value)
{
    return value * 5;
}

int ProcessG(int value)
{
    return value + 1;
}

void SendResult(int value)
{
    std::string s_msgr = std::to_string(value);
    int msg_size = s_msgr.size();
    std::cout << s_msgr << std::endl;

    send(Connection, (char*)&msg_size, sizeof(int), NULL);
    send(Connection, s_msgr.c_str(), msg_size, NULL);

    if (logFile.is_open()) {
        auto now = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(now);

        logFile << "[Client " << clientId << "][" << std::ctime(&end_time) << "] Sent result: " << value << std::endl;
    }
}

void ClientHandler() {
    int msg_size;
    std::string messageStr;
    while (true) {
        recv(Connection, (char*)&msg_size, sizeof(int), NULL);
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv(Connection, msg, msg_size, NULL);
        std::cout << msg << std::endl;

        messageStr = msg;
        bool foundIdF = messageStr.find("Client F") != std::string::npos;
        bool foundIdG = messageStr.find("Client G") != std::string::npos;

        if (foundIdF)
        {
            clientId = 'F';
            if (logFile.is_open()) {
                auto now = std::chrono::system_clock::now();
                std::time_t end_time = std::chrono::system_clock::to_time_t(now);

                logFile << "[Client F] Client F defined at " << std::ctime(&end_time) << std::endl;
            }
            continue;
        }
        if (foundIdG)
        {
            clientId = 'G';
            if (logFile.is_open()) {
                auto now = std::chrono::system_clock::now();
                std::time_t end_time = std::chrono::system_clock::to_time_t(now);

                logFile << "[Client G] Client G defined at " << std::ctime(&end_time) << std::endl;
            }
            continue;
        }        

        try
        {
            int value = std::stoi(messageStr);

            if (clientId != 'F' && clientId != 'G'&& logFile.is_open())
            {
                auto now = std::chrono::system_clock::now();
                std::time_t end_time = std::chrono::system_clock::to_time_t(now);

                logFile << "UNDEFINED CLIENT" << std::endl;
            }

            if (logFile.is_open()) {
                auto now = std::chrono::system_clock::now();
                std::time_t end_time = std::chrono::system_clock::to_time_t(now);
                logFile << "[Client " << clientId << "][" << std::ctime(&end_time) << "] Received argument " << value << ".Processing..." << std::endl;
            }

            if (clientId == 'F')
            {
                int result = ProcessF(value);
                SendResult(result);
            }
            else if (clientId == 'G')
            {
                int result = ProcessG(value);
                SendResult(result);
            }
        }
        catch (std::invalid_argument const& ex)
        {
            if (logFile.is_open()) {
                auto now = std::chrono::system_clock::now();
                std::time_t end_time = std::chrono::system_clock::to_time_t(now);
                logFile << "[Client " << clientId << "][" << std::ctime(&end_time) << "] Invalid Argument!" << std::endl;
            }
        }
        catch (std::out_of_range const& ex)
        {
            if (logFile.is_open()) {
                auto now = std::chrono::system_clock::now();
                std::time_t end_time = std::chrono::system_clock::to_time_t(now);
                logFile << "[Client " << clientId << "][" << std::ctime(&end_time) << "] Out of Integer Range!" << std::endl;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error!" << std::endl;
        exit(1);
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1053);
    addr.sin_family = AF_INET;

    Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr))) {
        std::cout << "Error: failed to connect to the server." << std::endl;
        return 1;
    }
    std::cout << "Client is connected!" << std::endl;
    
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

    std::string msgl;
    while (true) {
        /*std::getline(std::cin, msgl);
            if (msgl == "Random")
            {
                for (int i = 0; i < 50; i++) {
                    int i_msgr = rand();
                    std::string s_msgr = std::to_string(i_msgr);
                    int msg_size = s_msgr.size();
                    std::cout << s_msgr << std::endl;

                    if (logFile.is_open()) {
                        auto now = std::chrono::system_clock::now();
                        std::time_t end_time = std::chrono::system_clock::to_time_t(now);

                        logFile << "Sent DateTime: " << std::ctime(&end_time) << s_msgr << std::endl;
                    }

                
                    Sleep(10);
                }
                continue;
            }
            int msg_size = msgl.size();



            send(Connection, (char*)&msg_size, sizeof(int), NULL);
            send(Connection, msgl.c_str(), msg_size, NULL);
            Sleep(10);*/
    }

    system("pause");
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
