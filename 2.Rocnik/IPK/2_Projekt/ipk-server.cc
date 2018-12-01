#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;


string readMessage(int commSocket)
{
    char buffer[1];
    int readBuffSize = 1;
    string serverMessage;
    while (1)
    {
        read(commSocket, buffer, readBuffSize);
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

void sendExpression(int commSocket, string expression)
{
    // Send SOLVE - 7 / 3
    string solveMessage = "SOLVE " + expression + "\n";
    if (write(commSocket, solveMessage.c_str(), solveMessage.length()) < 0)
    {
        fprintf(stderr, "%s\n", "Error sending HW");
        exit(1);
    }

    string solveOutput = readMessage(commSocket);
    fprintf(stderr, "%s\n", solveOutput.c_str());
}

int main(int argc, char **argv)
{
    struct sockaddr_in serverAddr;
    string hostAdress = "localhost";
    int portNumber = 55555;

    // --------- CONNECTION ----------
    // Init socket
    int welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (welcomeSocket < 0)
    {
        fprintf(stderr, "%s\n", "Error establishing connection.");
        exit(1);
    }

    fprintf(stderr, "%s\n", "Server socket connection created...");

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(portNumber);

    // Binding socket
    if (bind(welcomeSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("");
        fprintf(stderr, "%s\n", "Error binding socket");
        exit(1);
    }

    fprintf(stderr, "%s\n", "Looking for clients...");

    // Listening socket
    if (listen(welcomeSocket, 1) < 0)
    {
        fprintf(stderr, "%s\n", "Error listening socket");
        exit(1);
    }

    socklen_t size = sizeof(serverAddr);
    while (1)
    {
        int commSocket = accept(welcomeSocket, (struct sockaddr*)&serverAddr, &size);
        if (commSocket > 0)
        {
            fprintf(stderr, "%s\n", "Client connected");

            // ---------- LOAD CLIENT 'HELLO' ----------
            // Read message
            char buffer[1];
            int readBuffSize = 1;
            string helloMessage;
            while (1)
            {
                read(commSocket, buffer, readBuffSize);
                string oneChar(buffer);
                helloMessage += oneChar;
                size_t foundEndHeader = helloMessage.find("\n");
                if (foundEndHeader != string::npos)
                {
                    break;
                }
            }

            // print hello message
            fprintf(stderr, "%s\n", helloMessage.c_str());

            // Send SOLVEs
            sendExpression(commSocket, "7 / 3");
            sendExpression(commSocket, "2 * 5");
            sendExpression(commSocket, "0 / 5");
            sendExpression(commSocket, "6 / 0");
            sendExpression(commSocket, "20 - 13");
            sendExpression(commSocket, "128 + 789");
            sendExpression(commSocket, "1225087900000 / 1000");
            sendExpression(commSocket, "12268 / 1000");

            // Send ASDCXV - Ignore
            string asdMessage = "ASDCXV UNKOWN\n";
            if (write(commSocket, asdMessage.c_str(), asdMessage.length()) < 0)
            {
                fprintf(stderr, "%s\n", "Error sending HW");
                exit(1);
            }

            sendExpression(commSocket, "128 + 789");

            // Send BYE
            string byeMessage = "BYE 58105c7af8dedda2b65fd16a7de3b12c\n";
            if (write(commSocket, byeMessage.c_str(), byeMessage.length()) < 0)
            {
                fprintf(stderr, "%s\n", "Error sending HW");
                exit(1);
            }

        }
    }

    return 0;
}
