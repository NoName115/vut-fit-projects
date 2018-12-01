/**
 * @Author Robert Kolcun (xkolcu00@stud.fit.vutbr.cz)
 * @date November, 2017
 * @brief Hlavickovy subor pre basefunc.cc
**/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sstream>
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <regex>

void print_stderr_message(std::string message);
unsigned char read_char(int socket_id);
void write_hex_message(std::string message);
int read_LL(int socket_id);
int read_message_id(int socket_id);
std::string read_message(int socket_id, int message_length);
std::string hex_to_string_char(unsigned char input_char);
std::string create_message_id(int message_id);
std::string create_ll(int message_ll);
std::string lower_string(std::string input_string);
