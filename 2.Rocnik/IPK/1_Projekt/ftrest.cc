// CLIENT

#include <ctime>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <sys/sendfile.h>
#include <signal.h>

#include "json/json.h"

using namespace std;


void handleError(int error)
{
	fprintf(stderr, "%s\n", "SIG - pipe");
}

string getDate()
{
	time_t rawtime;
	struct tm * timeinfo;
	char timeBuffer[80];
	time (&rawtime);

	timeinfo = localtime(&rawtime);
	strftime(timeBuffer, sizeof(timeBuffer), "%a, %d %b %Y %H:%M:%S %Z", timeinfo);
	string headerDate(timeBuffer);

	return headerDate;
}

long getFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

int main(int argc, char **argv)
{
	signal(SIGPIPE, handleError);

	struct sockaddr_in serverAddr;
	string hostAdress = "localhost";
	int portNumber = 6677;
	int bufsize = 1024;

	string command;
	string remotePath;
	string localPath;


	// --------- INPUT ARGUMENTS ----------
	// Check input arguments
	if (argc < 3 || argc > 4)
	{
		fprintf(stderr, "%s\n", "Invalid input arguments");
		exit(1);
	}

	// Parse input arguments
	if (!strcmp(argv[1], "del") || !strcmp(argv[1], "get") || !strcmp(argv[1], "put") ||
		!strcmp(argv[1], "lst") || !strcmp(argv[1], "mkd") || !strcmp(argv[1], "rmd"))
	{
		command = argv[1];
		remotePath = argv[2];

		if (argc == 3 && !strcmp(argv[1], "put"))
		{
			fprintf(stderr, "%s\n", "Invalid input arguments");
			exit(1);
		}
		else if (argc == 4)
		{
			localPath = argv[3];
		}
	}
	else
	{
		fprintf(stderr, "%s\n", "Invalid input arguments");
		exit(1);
	}

	// Parse data from input arguments
	if (argc == 3)
	{
		size_t findLastSlash = remotePath.find_last_of("/");
		localPath = remotePath.substr(findLastSlash + 1, remotePath.length() - findLastSlash);
	}

	size_t findIndex = remotePath.find(':');	// Find : in http
	remotePath.erase(findIndex, 3);				// Delete ://

	size_t findIndexDots = remotePath.find(':');	// Find : that separate port
	size_t findIndexSlash = remotePath.find('/');	// Find / after port

	try
	{
		portNumber = stoi(remotePath.substr(findIndexDots + 1, (findIndexSlash - findIndexDots)));
	}
	catch(std::invalid_argument& e)
	{
		fprintf(stderr, "%s\n", "Port error");
		exit(1);
	}

	hostAdress = remotePath.substr(findIndex, (findIndexDots - findIndex));
	remotePath = remotePath.substr(findIndexSlash, (remotePath.length() - findIndexSlash));


	// ----------- HEADER -----------
	string serverCommand;
	string serverAdress;
	string serverType;
	if (!strcmp(argv[1], "del"))
	{
		serverCommand = "DELETE";
		serverType = "file";

	}
	else if (!strcmp(argv[1], "rmd"))
	{
		serverCommand = "DELETE";
		serverType = "folder";
	}
	else if (!strcmp(argv[1], "get"))
	{
		serverCommand = "GET";
		serverType = "file";
	}
	else if (!strcmp(argv[1], "lst"))
	{
		serverCommand = "GET";
		serverType = "folder";
	}
	else if (!strcmp(argv[1], "put"))
	{
		serverCommand = "PUT";
		serverType = "file";
	}
	else if (!strcmp(argv[1], "mkd"))
	{
		serverCommand = "PUT";
		serverType = "folder";
	}

	// Complete header
	serverAdress = remotePath + "?type=" + serverType;
	string acceptType = "application/json";
	if (!strcmp(argv[1], "get"))
	{
		acceptType = "application/octet-stream";
	}

	string headerOutput = serverCommand + " " + serverAdress + " HTTP/1.1\r\n";
	headerOutput += "Host: " + hostAdress + "\r\n";
	headerOutput += "Date: " + getDate() + "\r\n";
	headerOutput += "Accept: " + acceptType + "\r\n";
	headerOutput += "Accept-Encoding: identity";

	if (!strcmp(argv[1], "put"))
	{
		headerOutput += "\r\nContent-Type: application/octet-stream\r\n";
		headerOutput += "Content-Length: " + to_string(getFileSize(localPath));
	}

	headerOutput += "\r\n\r\n";

	// Check put file
	if (command.compare("put") == 0)
	{
		FILE *putFile = fopen(localPath.c_str(), "rb");
		if (putFile == NULL)
		{
			fprintf(stderr, "%s\n", "Error opening file ...");
			exit(1);
		}
	}


	// --------- CONNECTION ----------
	// Init socket
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0)
	{
		fprintf(stderr, "%s\n", "Error creating socket...");
		exit(1);
	}

	struct hostent *hostname = gethostbyname(hostAdress.c_str());
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNumber);

	memcpy(&serverAddr.sin_addr, hostname->h_addr, hostname->h_length);

	// Connect to server
	if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
	{
		fprintf(stderr, "%s\n", "Error connecting to server...");
		exit(1);
	}

	// Write client-header
	if (write(clientSocket, headerOutput.c_str(), headerOutput.length()) < 0)
	{
		fprintf(stderr, "%s\n", "Error sending HW");
	}

	// Write file to socket
	if (command.compare("put") == 0)
	{
		off_t offset = 0;
		int retCode;
		int remaining = getFileSize(localPath);
		int sendFile = open(localPath.c_str(), O_RDONLY);

		while (remaining > 0 && (retCode = sendfile(clientSocket, sendFile, &offset, bufsize)) > 0)
		{
			remaining -= retCode;
		}
		close(sendFile);
	}



	// --------- SERVER HEADER ---------
	// Read server header
	char buffer[1];
	int readBuffSize = 1;
	string serverHeader;
	while (1)
	{
		read(clientSocket, buffer, readBuffSize);
		string oneChar(buffer);
		serverHeader += oneChar;
		size_t foundEndHeader = serverHeader.find("\r\n\r\n");

		if (foundEndHeader != string::npos)
		{
			break;
		}
	}

	// Parse header
	size_t findIndexSpace = serverHeader.find(" ");
	serverHeader.erase(0, findIndexSpace + 1);
	findIndexSpace = serverHeader.find(" ");

	int returnCode = stoi(serverHeader.substr(0, findIndexSpace));

	// Get Content-Length
	for (int i = 0; i < 3; ++i)
	{
		size_t findIndexRN = serverHeader.find("\r\n");
		serverHeader.erase(0, findIndexRN + 2);
	}
	size_t findIndexRN = serverHeader.find("\r\n");
	string contentLength = "Content-Length: ";
	int sizeToRead = stoi(serverHeader.substr(contentLength.length(), findIndexRN - contentLength.length()));

	// Write server output to stdout
	if (returnCode != 200 || (returnCode == 200 && command.compare("lst") == 0))
	{
		char jsonBuffer[bufsize];
		read(clientSocket, jsonBuffer, bufsize);
		string myJson(jsonBuffer);

		Json::Value root;
		Json::Reader reader;
		bool parsingOk = reader.parse(jsonBuffer, root);
		if (!parsingOk)
		{
			fprintf(stderr, "%s\n", "Error Json parsing...");
			close(clientSocket);
			exit(1);
		}

		if (command.compare("lst") == 0 && returnCode == 200)
		{
			try
			{
				cout << root.get("_body", "UTF-8").asString();
			}
			catch (std::exception &e)
			{
				cout << root << endl;
			}
		}
		else
		{
			cout << root.get("_error", "UTF-8").asString() << endl;
		}
	}


	// --------- GET FILE FROM SERVER ---------
	if (returnCode == 200 && command.compare("get") == 0)
	{
		int sizeRead = 0;
		char fileBuffer[bufsize];
		memset(fileBuffer, 0, bufsize);

		FILE *file = fopen(localPath.c_str(), "wb");
		if (file == NULL)
		{
			fprintf(stderr, "%s\n", "Error opening file...");
			close(clientSocket);
			exit(1);
		}

		size_t retCode;
		while((retCode = read(clientSocket, fileBuffer, bufsize)) > 0 || (sizeRead < sizeToRead))
		{
			fwrite(fileBuffer, sizeof(char), retCode, file);
			sizeRead += retCode;
		}

		fclose(file);
	}

	close(clientSocket);
	return 0;
}
