// SERVER

#include <ctime>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <sys/sendfile.h>

using namespace std;


pair <int, string> checkUserName(string inputName)
{
	// error - User Account Not Found

	struct stat info;
	if (stat(inputName.c_str(), &info) != 0)
	{
		return pair<int, string> (404, "User Account Not Found");
	}
	else if (info.st_mode & S_IFDIR)
	{
		return pair<int, string> (200, "OK");
	}
	else
	{
		return pair<int, string> (400, "Not a directory.");
	}
}

// Operations put, mkd
pair <int, string> requestPut(string remotePath, string typeCom, int fileSize, int clientSocket)
{
	// mkd - Already exists.
	// put - Already exists.

	struct stat info;
	if (stat(remotePath.c_str(), &info) != 0)
	{
		if (typeCom.compare("file") == 0)
		{
			// Command - put
			int bufsize = 1024;
			int sizeRead = 0;
			char fileBuffer[bufsize];
			memset(fileBuffer, 0, bufsize);

			FILE *file = fopen(remotePath.c_str(), "wb");
			if (file == NULL)
			{
				fprintf(stderr, "%s\n", "Error opening file...");
				exit(1);
			}

			size_t retCode;
			while(sizeRead < fileSize)
			{
				retCode = read(clientSocket, fileBuffer, bufsize);
				fwrite(fileBuffer, sizeof(char), retCode, file);
				sizeRead += retCode;
			}
			fclose(file);
		}
		else
		{
			// Command - mkd
			if (mkdir(remotePath.c_str(), S_IRWXU) != 0)
			{
				return pair<int, string> (400, "Already exists.");
			}
		}
	}
	else if (info.st_mode)
	{
		return pair<int, string> (400, "Already exists.");
	}

	return pair<int, string> (200, "OK");
}

// Operations get, lst
pair <int, string> requestGet(string remotePath, string typeCom)
{
	// get - Not a file., File not found.
	// lst - Not a directory., Directory not found.

	struct stat info;
	if (stat(remotePath.c_str(), &info) != 0)
	{
		if (typeCom.compare("file") == 0)
		{
			return pair<int, string> (404, "File not found.");
		}
		else
		{
			return pair<int, string> (404, "Directory not found.");
		}
	}
	else if ((info.st_mode & S_IFDIR) && (typeCom.compare("file") == 0))
	{
		return pair<int, string> (400, "Not a file.");
	}
	else if ((info.st_mode & S_IFREG) && (typeCom.compare("folder") == 0))
	{
		return pair<int, string> (400, "Not a directory.");
	}

	if (typeCom.compare("file") == 0)
	{
		// Command get
		// Dole, po zapisani hlavicky so socketu
		return pair<int, string> (200, "OK");
	}
	else
	{
		// Command - lst
		DIR *dpdf;
		struct dirent *epdf;
		string lsOutput;

		dpdf = opendir(remotePath.c_str());
		if (dpdf != NULL)
		{
			while (epdf = readdir(dpdf))
			{
				string fileName(epdf->d_name);

				if (fileName.compare(".") == 0 || fileName.compare("..") == 0)
				{
					continue;
				}
				else
				{
					lsOutput += fileName + " ";
				}
			}
		}

		// Remove last space
		if (lsOutput.length() != 0)
		{
			lsOutput.erase(lsOutput.length() - 1, 1);
			lsOutput += '\n';
		}

		return pair<int, string> (200, lsOutput);
	}

	return pair<int, string> (200, "OK");
}

// Operations del, rmd
pair <int, string> requestDelete(string remotePath, string typeCom)
{
	// del - Not a file., File not found.
	// rmd - Not a directory., Directory not found., Directory not empty.

	struct stat info;
	if (stat(remotePath.c_str(), &info) != 0)
	{
		if (typeCom.compare("file") == 0)
		{
			return pair<int, string> (404, "File not found.");
		}
		else
		{
			return pair<int, string> (404, "Directory not found.");
		}
	}
	else if ((info.st_mode & S_IFDIR) && (typeCom.compare("file") == 0))
	{
		return pair<int, string> (400, "Not a file.");
	}
	else if ((info.st_mode & S_IFREG) && (typeCom.compare("folder") == 0))
	{
		return pair<int, string> (400, "Not a directory.");
	}

	if (typeCom.compare("file") == 0)
	{
		// Command del
		if (remove(remotePath.c_str()) != 0)
		{
			return pair<int, string> (404, "File not found.");
		}
	}
	else
	{
		// Command rmd
		if (remove(remotePath.c_str()) != 0)
		{
			return pair<int, string> (400, "Directory not empty");
		}
	}

	return pair<int, string> (200, "OK");
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
	struct sockaddr_in serverAddr;
	int portNumber = 6677;			// default port
	int rc;
	int bufsize = 1024;

	string rootFolder = "./";

	// --------- INPUT ARGUMENTS ----------
	// Check input arguments
	if (argc == 2 || argc == 4 || argc > 5)
	{
		fprintf(stderr, "%s\n", "Invalid input arguments");
		exit(1);
	}

	// Parse input arguments
	for (int i = 1; i < argc; i += 2)
	{
		// root-folder
		if (!strcmp(argv[i], "-r"))
		{
			rootFolder += argv[i + 1] + string("/");
		}
		// port
		else if (!strcmp(argv[i], "-p"))
		{
			try
			{
				portNumber = stoi(argv[i + 1]);
			}
			catch(std::invalid_argument& e)
			{
				fprintf(stderr, "%s\n", "Invalid input arguments");
				exit(1);
			}
		}
		else
		{
			fprintf(stderr, "%s\n", "Invalid input argument");
			exit(1);
		}
	}

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
	if ((rc = bind(welcomeSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr))) < 0)
	{
		perror("");
		fprintf(stderr, "%s\n", "Error binding socket");
		exit(1);
	}

	fprintf(stderr, "%s\n", "Looking for clients...");

	// listening socker
	if ((listen(welcomeSocket, 1)) < 0)
	{
		fprintf(stderr, "%s\n", "Error listening socket");
		exit(1);
	}

	socklen_t size = sizeof(serverAddr);
	while (1)
	{
		sleep(2);
		int commSocket = accept(welcomeSocket, (struct sockaddr*)&serverAddr, &size);

		if (commSocket > 0)
		{
			fprintf(stderr, "%s\n", "Client connected");


			// ---------- LOAD CLIENT HEADER ----------
			// Read header
			char buffer[1];
			int readBuffSize = 1;
			string headerClient;

			while (1)
			{
				read(commSocket, buffer, readBuffSize);
				string oneChar(buffer);
				headerClient += oneChar;
				size_t foundEndHeader = headerClient.find("\r\n\r\n");
				if (foundEndHeader != string::npos)
				{
					break;
				}
			}

			// Parse header
			// Get COMMAND
			size_t findIndexSpace = headerClient.find(" ");
			size_t findIndexSlash = headerClient.find("/");
			string command = headerClient.substr(0, findIndexSpace);

			// Get USER_NAME
			headerClient.erase(0, findIndexSlash + 1);
			findIndexSlash = headerClient.find("/");
			string userName = headerClient.substr(0, findIndexSlash);

			// Get REMOTE_PATH
			size_t findIndexQuestionMark = headerClient.find("?");
			string remotePath = rootFolder + userName + headerClient.substr(findIndexSlash, findIndexQuestionMark - findIndexSlash);

			headerClient.erase(0, findIndexQuestionMark);		// Delete command and remotePath
			findIndexSpace = headerClient.find(" ");			// Find Space before HTTP...
			string typeCom = headerClient.substr(6, findIndexSpace - 6);	// get type - FILE or FOLDER

			// If client upload file, get contentLength from header
			int inputContentLength = -1;
			if (command.compare("PUT") == 0 && typeCom.compare("file") == 0)
			{
				size_t findIndexContent = headerClient.find("Content-Length: ");
				size_t findIndexSize = headerClient.find("\r\n", findIndexContent);

				string content = "Content-Length: ";
				inputContentLength = stoi(headerClient.substr(findIndexContent + content.length(), findIndexSize - (findIndexContent + content.length())));
			}


			// ---------- COMMANDS ----------
			// Check userName
			pair <int, string> catchError = checkUserName(userName);
			if (catchError.first == 200)
			{
				// Call function for certain request
				if (command.compare("PUT") == 0)
				{
					catchError = requestPut(remotePath, typeCom, inputContentLength, commSocket);
				}
				else if (command.compare("GET") == 0)
				{
					catchError = requestGet(remotePath, typeCom);
				}
				else if (command.compare("DELETE") == 0)
				{
					catchError = requestDelete(remotePath, typeCom);
				}
			}


			// ---------- SERVER HEADER ----------
			// Variables
			string requestResult = "HTTP/1.1 " + to_string(catchError.first);
			string contentType = "application/json";
			string content;
			if (catchError.first == 200)
			{
				requestResult += " OK";
				if (command.compare("GET") == 0 && typeCom.compare("file") == 0)
				{
					contentType = "application/octet-stream";
				}
			}
			else if (catchError.first == 404)
			{
				requestResult += " Not Found";
			}
			else
			{
				requestResult += " Bad Request";
			}

			string serverHeader = requestResult + "\r\n";
			serverHeader += "Date: " + getDate() + "\r\n";
			serverHeader += "Content-Type: " + contentType + "\r\n";

			// Add error output
			if (catchError.first != 200)
			{
				content = "{\"_error\" : \"" + catchError.second + "\"}";
			}
			// Add lst output
			if (catchError.first == 200 && command.compare("GET") == 0 && typeCom.compare("folder") == 0)
			{
				content = "{\"_body\" : \"" + catchError.second + "\"}";
			}

			// Get length of output
			int contentLength = content.length();
			if (catchError.first == 200 && command.compare("GET") == 0 && typeCom.compare("file") == 0)
			{
				contentLength = getFileSize(remotePath);
			}

			serverHeader += "Content-Length: " + to_string(contentLength) + "\r\n";
			serverHeader += "Content-Encoding: identity\r\n\r\n";
			if (!content.empty())
			{
				serverHeader += content;
			}

			// Write header
			if (write(commSocket, serverHeader.c_str(), serverHeader.length()) < 0)
			{
				fprintf(stderr, "%s\n", "Error sending HW");
				exit(1);
			}


			// --------- SEND FILE TO CLIENT ---------
			if (catchError.first == 200 && command.compare("GET") == 0 && typeCom.compare("file") == 0)
			{
				off_t offset = 0;
				int retCode;
				int remaining = contentLength;
				int file = open(remotePath.c_str(), O_RDONLY);

				while (remaining > 0 && (retCode = sendfile(commSocket, file, &offset, bufsize)) > 0)
				{
					remaining -= retCode;
				}

				// Wait until client read all data
				shutdown(commSocket, SHUT_WR);
				close(file);
			}
			
			//close(commSocket);
			fprintf(stderr, "%s\n", "Client finished");
		}
	}

	return 0;
}
