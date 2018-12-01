#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include <math.h>

#include "md5.h"

using namespace std;


string readMessage(int clientSocket)
{
    char buffer[1];
    int readBuffSize = 1;
    string serverMessage;
    while (1)
    {
        read(clientSocket, buffer, readBuffSize);
        string oneChar(buffer);
        serverMessage += oneChar;
        size_t foundEndHeader = serverMessage.find("\n");

        if (foundEndHeader != string::npos)
        {
            serverMessage.erase(serverMessage.length() - 1, 1);
            break;
        }
    }

    return serverMessage;
}

pair <double, string> solveExpression(int firstNumber, int secondNumber, string operatorNumber)
{
    if (operatorNumber.compare("+") == 0)
    {
        double result = (double) firstNumber + (double) secondNumber;
        return pair<double, string> (result, "OK");
    }
    else if (operatorNumber.compare("-") == 0)
    {
        double result = (double) firstNumber - (double) secondNumber;
        return pair<double, string> (result, "OK");
    }
    else if (operatorNumber.compare("*") == 0)
    {
        double result = (double) firstNumber * (double) secondNumber;
        return pair<double, string> (result, "OK");
    }
    else if (operatorNumber.compare("/") == 0)
    {
        if (secondNumber != 0)
        {
            double result = (double) firstNumber / (double) secondNumber;
            return pair<double, string> (result, "OK");
        }
        else
        {
            return pair<double, string> (0, "ERROR");
        }
    }
    else
    {
        return pair<double, string> (0, "ERROR");
    }
}

int main(int argc, char **argv)
{
    string login = "xkolcu00";

    struct sockaddr_in serverAddr;
    string hostAdress = "";
    string portNumber = "55555";

    if (argc < 2 || argc > 2)
    {
        fprintf(stderr, "%s\n", "Invalid input arguments");
        exit(1);
    }

    hostAdress = argv[1];
    string helloMessage = "HELLO " + md5(login) + "\n";

    // IPv4, IPv6
    int status;
    struct addrinfo hints;
    struct addrinfo *res;   // will point to the results

    memset(&hints, 0, sizeof hints);    // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;        // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;    // TCP stream sockets

    if ((status = getaddrinfo(hostAdress.c_str(), portNumber.c_str(), &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    // Create socket
    int clientSocket = socket(res->ai_family, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        fprintf(stderr, "%s\n", "Error creating socket...");
        exit(1);
    }

    // Connect to server
    if (connect(clientSocket, res->ai_addr, res->ai_addrlen) < 0)
    {
        fprintf(stderr, "%s\n", "Error connecting to server...");
        exit(1);
    }

    // Send HELLO [id]\n
    if (write(clientSocket, helloMessage.c_str(), helloMessage.length()) < 0)
    {
        fprintf(stderr, "%s\n", "Error sending HW");
        exit(1);
    }

    // Read messages
    bool isBye = false;
    while (!isBye)
    {
        string serverMessage = readMessage(clientSocket);

        // Parse message to 2 sections
        size_t findIndexSpace = serverMessage.find(' ');
        string firstMessage = serverMessage.substr(0, findIndexSpace);
        string secondMessage = serverMessage.substr(
            findIndexSpace + 1,
            serverMessage.length() - findIndexSpace
            );

        if (firstMessage.compare("BYE") == 0)
        {
            isBye = true;
            fprintf(stdout, "%s", secondMessage.c_str());
        }
        else if (firstMessage.compare("SOLVE") == 0)
        {
            size_t indexFirstSpace = secondMessage.find(' ');
            size_t indexSecondSpace = secondMessage.find(' ', indexFirstSpace + 1);

            // Parse numbers and operator
            string firstNumberString = secondMessage.substr(
                0,
                indexFirstSpace
                );
            string operatorNumber = secondMessage.substr(
                indexFirstSpace + 1,
                1
                );
            string secondNumberString = secondMessage.substr(
                indexSecondSpace + 1,
                secondMessage.length() - indexSecondSpace - 1
                );

            pair <double, string> expressionResult;
            try
            {
                int firstNumber = stoi(firstNumberString);
                int secondNumber = stoi(secondNumberString);

                expressionResult = solveExpression(
                    firstNumber,
                    secondNumber,
                    operatorNumber
                );
            }
            catch (std::exception& err)
            {
                expressionResult = pair <double, string> (0, "ERROR");
            }

            string resultMessage = "";
            if (expressionResult.second.compare("ERROR") == 0)
            {
                resultMessage = "RESULT ERROR\n";
            }
            else
            {
                double correctNumber = floor(expressionResult.first * 100) / 100;
                stringstream stream;
                stream << fixed << setprecision(2) << correctNumber;
                string fixedResult = stream.str();
                resultMessage = "RESULT " + fixedResult + "\n";
            }

            // Send resultMessage to server
            if (write(clientSocket, resultMessage.c_str(), resultMessage.length()) < 0)
            {
                fprintf(stderr, "%s\n", "Error sending HW");
                exit(1);
            }
        }
    }

    close(clientSocket);
    freeaddrinfo(res);

    return 0;
}
