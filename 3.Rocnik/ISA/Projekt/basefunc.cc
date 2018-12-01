/**
 * @Author Robert Kolcun (xkolcu00@stud.fit.vutbr.cz)
 * @date November, 2017
 * @brief Zakladne funkcie pre ldapserver
 *
 * Subor obsahuje zakladne funkcie pre citanie a zapisaovanie
 * do socketov. Obahuje funkcie pre pomocne vystupy pouzite
 * pri debugovaniu. Dalej funkcie pre citanie dlzok(LL),
 * message_id a retazcov v ldap spravach.
**/

#include "basefunc.h"
#include <algorithm>

using namespace std;

/**
 * Vypisovanie chyb
 * @param message Vstupny retazec pre vypis
**/
void print_stderr_message(string message)
{
    fprintf(stderr, "%s\n", message.c_str());
}

/**
 * Vypise vstupnu spravu v hexadecimalnom formate
 * @param message Vstupny retazec pre vypis
**/
void write_hex_message(string message)
{
    for(string::size_type i = 0; i < message.size(); ++i)
    {
        unsigned char charicek = message[i];
        fprintf(stderr, "0x%x ", charicek);
    }
    fprintf(stderr, "\n");
}

/**
 * Konvertuje vstupny charakter do retazca
 * @param input_char Vstupny znak
 * @return string Vystupny retazec
**/
string hex_to_string_char(unsigned char input_char)
{
    string tmp(1, input_char);
    return tmp;
}

/**
 * Precitanie jedneho znaku zo socketu
 * @param socket_id Socket
 * @return unsigned char Precitany znak
**/
unsigned char read_char(int socket_id)
{
    unsigned char buffer[1];
    int read_buffsize = 1;
    read(socket_id, buffer, read_buffsize);

    unsigned char answer = buffer[0];

#ifdef NDEBUG
    fprintf(stderr, "0x%x\n", answer);
#endif

    return answer;
}

/**
 * Precita LL zo socketu
 * @param socket_id Socket
 * @return int Precitana dlzka
**/
int read_LL(int socket_id)
{
    int ll = (int) read_char(socket_id);
    if (ll > 128)
    {
        int number_of_bytes = ll - 128;

        fprintf(stderr, "NOB: %d\n", number_of_bytes);

        ll = 0;
        for (int i = 1; i <= number_of_bytes; ++i)
        {
            int one_byte = (int) read_char(socket_id);
            ll += one_byte << (8 * (number_of_bytes - i));
        }
    }

#ifdef NDEBUG
    fprintf(stderr, "Length: %d\n", ll);
#endif

    return ll;
}

/**
 * Precita message_id zo socketu
 * @param socket_id Socket
 * @return -1 pri chybe, inac id spravy
**/
int read_message_id(int socket_id)
{
    int number_of_bytes = (int) read_char(socket_id);
    if (number_of_bytes < 1 || number_of_bytes > 4)
    {
        print_stderr_message("Invalid number of bytes for message_id");
        return -1;
    }

    // Read message_id
    int message_id = 0;
    for (int i = 1; i <= number_of_bytes; ++i)
    {
        int one_byte = (int) read_char(socket_id);
        message_id += one_byte << (8 * (number_of_bytes - i));
    }

#ifdef NDEBUG
    fprintf(stderr, "Message_ID: %d\n", message_id);
#endif

    return message_id;
}

/**
 * Precita retazec zo socketu podla vstupnej dlzky
 * @param socker_id Socket
 * @param message_legnth Dlzka retazca
 * @return string Precitany retazec
**/
string read_message(int socket_id, int message_length)
{
    unsigned char buffer;
    int read_buffsize = 1;
    string output_message = "";

    for (int i = 0; i < message_length; ++i)
    {
        read(socket_id, &buffer, read_buffsize);
        output_message += buffer;
    }

#ifdef NDEBUG
    fprintf(stderr, "%s\n", output_message.c_str());
#endif

    return output_message;
}

/**
 * Vytvorenie id spravy do hexadecimalneho formatu
 * @param message_id Id spravy pre konvertovanie
 * @return string Id spravy v hex. formate
**/
string create_message_id(int message_id)
{
    string string_message_id = "";
    int number_of_bytes = ceil((int) (log2(message_id) + 1) / 8.0);
    string_message_id += hex_to_string_char((unsigned char) number_of_bytes);

    for (int i = 1; i <= number_of_bytes; ++i)
    {
        int id = message_id;
        unsigned char one_char = message_id >> (8 * (number_of_bytes - i));
        string_message_id += hex_to_string_char(one_char);
    }

    return string_message_id;
}

/**
 * Vyvorenie dlzky do hex. formatu
 * @param message_ll Dlzka spravy
 * @return string Dlzka spravy v hex. formate
**/
string create_ll(int message_ll)
{
    if (message_ll <= 127)
    {
        return hex_to_string_char((unsigned char) message_ll);
    }
    else
    {
        int number_of_bytes = ceil((int) (log2(message_ll) + 1) / 8.0);
        string message_ll_string = hex_to_string_char(0x80 + (unsigned char) number_of_bytes);
        for (int i = 1; i <= number_of_bytes; ++i)
        {
            message_ll_string += hex_to_string_char(
                (unsigned char) (message_ll >> (8 * (number_of_bytes - i)))
            );
        }

        return message_ll_string;
    }
}

/**
 * Konvertovanie retazca na male pismena
 * @param input_string Retazec pre konvertovanie
 * @return string Retazec s malimi pismenami
**/
string lower_string(string input_string)
{
    transform(
        input_string.begin(),
        input_string.end(),
        input_string.begin(),
        ::tolower
    );

    return input_string;
}
