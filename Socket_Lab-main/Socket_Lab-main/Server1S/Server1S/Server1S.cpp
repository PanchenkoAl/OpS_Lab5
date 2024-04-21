// Server1S.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include "winsock2.h"
#include <string>
#include <string.h>
#include <fstream>
#include <chrono>

#pragma warning(disable: 4996)

//SOCKET Connections[100];
SOCKET ConnectionF;
SOCKET ConnectionG;

//int counter = 0;
int resultF = 0;
int resultG = 0;

std::ofstream logFile("D:\\Server.txt", std::ios::app);

int GetResult();
void SendValueToClients(int value);

void ClientHandler(char index) {
    int msg_size;
    int value = 0;
    bool isSimple = false;
    std::string msgr;
    std::string s_msg;
    int msgr_size;

    SOCKET Connection;

    if (index == 'F')
    {
        Connection = ConnectionF;
    }
    else
    {
        Connection = ConnectionG;
    }
    
    bool hasValue = false;
    int valueClient;
    while (true) 
    {
        std::cout << "Enter value for calculations: ";
        int value;
        std::cin >> value;


        std::string userInput;
        bool newCalculationsAvailable = true;
        std::chrono::steady_clock::time_point startTime;
        bool hasValueF = false;
        bool hasValueG = false;

        int valueF;
        int valueG;

        bool valueFUpdate = false;
        bool valueGUpdate = false;

        SendValueToClients(value);

        if (!hasValueF)
        {
            int msg_size;
            recv(ConnectionF, (char*)&msg_size, sizeof(int), NULL);
            char* msg = new char[msg_size + 1];
            msg[msg_size] = '\0';
            recv(ConnectionF, msg, msg_size, NULL);

            try
            {
                valueF = std::stoi(msg);
                valueFUpdate = true;
                hasValueF = true;
            }
            catch (std::invalid_argument const& ex)
            {

            }
            catch (std::out_of_range const& ex)
            {

            }
        }

        if (!hasValueG)
        {
            int msg_size;
            recv(ConnectionG, (char*)&msg_size, sizeof(int), NULL);
            char* msg = new char[msg_size + 1];
            msg[msg_size] = '\0';
            recv(ConnectionG, msg, msg_size, NULL);

            try
            {
                valueG = std::stoi(msg);
                valueGUpdate = true;
                hasValueG = true;
            }
            catch (std::invalid_argument const& ex)
            {

            }
            catch (std::out_of_range const& ex)
            {

            }
        }

        if (hasValueF && valueF && valueFUpdate && valueGUpdate)
        {
            std::cout << "f(x) || g(x) = true" << std::endl;
            valueFUpdate = false;
            valueGUpdate = false;
            newCalculationsAvailable = true;
            continue;
        }
        else if (hasValueF && hasValueG && valueFUpdate && valueGUpdate)
        {
            std::cout << "f(x) || g(x) = " << (valueF || valueG) << std::endl;
            valueFUpdate = false;
            valueGUpdate = false;
            newCalculationsAvailable = true;
            continue;
        }

        auto now = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();

        if (elapsedTime > 5)
        {
            std::cout << "Clients seam to be stuck. Press 1 to continue, press 0 to restart.\n";
            int input;
            std::cin >> input;
            if (input)
            {
                startTime = std::chrono::steady_clock::now();
            }
            else
            {
                newCalculationsAvailable = true;
                continue;
            }
        }


        //recv(Connection, (char*)&msg_size, sizeof(int), NULL);
        //char* msg = new char[msg_size + 1];
        //msg[msg_size] = '\0';
        //recv(Connection, msg, msg_size, NULL);
        //std::cout << msg << std::endl;
        //s_msg = msg;

        ////send input for F
        //send(Connections[0], (char*)&msgr_size, sizeof(int), NULL);
        //send(Connections[0], msgr.c_str(), msgr_size, NULL);
        ////F(s_msg);

        ////send input for G
        //send(Connections[1], (char*)&msgr_size, sizeof(int), NULL);
        //send(Connections[1], msgr.c_str(), msgr_size, NULL);
        ////G(s_msg);
        //if (logFile.is_open()) {
        //    auto now = std::chrono::system_clock::now();
        //    std::time_t end_time = std::chrono::system_clock::to_time_t(now);

        //    logFile << "Received DateTime: " << std::ctime(&end_time) << msgr << std::endl;
        //}

        //for (int i = 0; i < counter; i++) {
        //    send(Connections[i], (char*)&msgr_size , sizeof(int), NULL);
        //    send(Connections[i], msgr.c_str(), msgr_size, NULL);
        //}
        //delete[] msg;
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

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);

    SOCKET newConnection;
    bool isClientFConnected = false;
    bool isClientGConnected = false;
    for (int i = 0; i < 100; i++) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

        if (newConnection == 0) {
            std::cout << "No valid connection." << std::endl;
        }
        else {
            std::cout << "Client Connected!" << std::endl;
            if (!isClientFConnected)
            {
                ConnectionF = newConnection;
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)('F'), NULL, NULL);

                std::string msgr = "Client F";
                int msgr_size = msgr.size();

                send(ConnectionF, (char*)&msgr_size, sizeof(int), NULL);
                send(ConnectionF, msgr.c_str(), msgr_size, NULL);
                std::cout << "Client F assigned." << std::endl;
                isClientFConnected = true;
            }
            else if (!isClientGConnected)
            {
                ConnectionG = newConnection;
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)('G'), NULL, NULL);

                std::string msgr = "Client G";
                int msgr_size = msgr.size();

                send(ConnectionG, (char*)&msgr_size, sizeof(int), NULL);
                send(ConnectionG, msgr.c_str(), msgr_size, NULL);                
                std::cout << "Client G assigned." << std::endl;
                isClientGConnected = true;
            }
            else
            {
                std::cout << "Client connection ignored!" << std::endl;
            }
        }
    }
    
    std::string userInput;
    bool newCalculationsAvailable = true;
    std::chrono::steady_clock::time_point startTime;
    bool hasValueF = false;
    bool hasValueG = false;

    int valueF;
    int valueG;
    
    while (true)
    {
        if (newCalculationsAvailable)
        {
            std::cout << "Input some number:" << std::endl;

            int value;
            std::cin >> value;

            startTime = std::chrono::steady_clock::now();

            newCalculationsAvailable = false;

            SendValueToClients(value);
        }
        //Server waits for result;
        else 
        {
            if (!hasValueF)
            {
                int msg_size;
                recv(ConnectionF, (char*)&msg_size, sizeof(int), NULL);
                char* msg = new char[msg_size + 1];
                msg[msg_size] = '\0';
                recv(ConnectionF, msg, msg_size, NULL);

                try
                {
                    valueF = std::stoi(msg);
                    hasValueF = true;
                }
                catch (std::invalid_argument const& ex)
                {
                    
                }
                catch (std::out_of_range const& ex)
                {
                    
                }
            }

            if (!hasValueG)
            {
                int msg_size;
                recv(ConnectionG, (char*)&msg_size, sizeof(int), NULL);
                char* msg = new char[msg_size + 1];
                msg[msg_size] = '\0';
                recv(ConnectionG, msg, msg_size, NULL);

                try
                {
                    valueG = std::stoi(msg);
                    hasValueG = true;
                }
                catch (std::invalid_argument const& ex)
                {

                }
                catch (std::out_of_range const& ex)
                {

                }
            }

            if (hasValueF && valueF)
            {            
               std::cout << "f(x) || g(x) = true" << std::endl;
               newCalculationsAvailable = true;
               continue;
            }
            else if (hasValueF && hasValueG)
            {
               std::cout << "f(x) || g(x) = " << (valueF || valueG) << std::endl;
               newCalculationsAvailable = true;
               continue;
            }

            auto now = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
                    
            if (elapsedTime > 5)
            {
                std::cout << "Clients seam to be stuck. Press 1 to continue, press 0 to restart.\n";
                int input;
                std::cin >> input;
                if (input)
                {
                    startTime = std::chrono::steady_clock::now();
                }
                else
                {
                    newCalculationsAvailable = true;
                    continue;
                }
            }
        }
    }
    system("pause");
    return 0;
}

void SendValueToClients(int value)
{
    if (ConnectionF && ConnectionG)
    {
        std::string s_msgr = std::to_string(value);
        int msg_size = s_msgr.size();
        std::cout << s_msgr << std::endl;

        send(ConnectionF, (char*)&msg_size, sizeof(int), NULL);
        send(ConnectionF, s_msgr.c_str(), msg_size, NULL);

        send(ConnectionG, (char*)&msg_size, sizeof(int), NULL);
        send(ConnectionG, s_msgr.c_str(), msg_size, NULL);

        std::cout << "Server sent value " << value << std::endl;
    }
    else
    {
        std::cout << "Server has less than two clients!" << std::endl;
    }
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
