/**
 * @Author Robert Kolcun (xkolcu00@stud.fit.vutbr.cz)
 * @date November, 2017
 * @brief Funkcie pre spracovanie poziadakov
 *
 * Funkcie pre vytvorenie ldap hlavicky, ldap_result,
 * poslanie odpovede klientovy, poslanie vysledkov vyhladavania
 * vytvorenie tychto sprav.
**/

#include "responser.h"

using namespace std;

/**
 * Vytvorenie zakladnej hlavicky ldap spravy
 * @param message_id ID spravy
 * @param protocol Typ protokolu
 * @return string Hlavicka spravy v hex. formate
**/
string create_ldap_header(int message_id, unsigned char protocol)
{
    string ldap_header = "";
    ldap_header += hex_to_string_char(0x02);
    ldap_header += create_message_id(message_id);
    ldap_header += hex_to_string_char(protocol);

    return ldap_header;
}

/**
 * Vyvorenie ldap_result spravy
 * @return string ldap_result sprava v hex. formate
**/
string create_ldap_result()
{
    string ldap_result = "";
    ldap_result += hex_to_string_char(0x0A);
    ldap_result += hex_to_string_char(0x01);
    ldap_result += hex_to_string_char(0x00);
    ldap_result += hex_to_string_char(0x04);
    ldap_result += hex_to_string_char(0x00);
    ldap_result += hex_to_string_char(0x04);
    ldap_result += hex_to_string_char(0x00);

    return ldap_result;
}

/**
 * Zapisanie spravy do socketu
 * @param socket_id Socket
 * @param message Sprava pre zapis
 * @return false - pri chybe, inac true
**/
bool send_response(int socket_id, string message)
{
#ifdef NDEBUG
    fprintf(stderr, "%s\n", "Writing message:");
    write_hex_message(message);
#endif

    if (write(socket_id, message.c_str(), message.length()) < 0)
    {
        print_stderr_message("Error while writing to socket");
        return false;
    }

    return true;
}

/**
 * Poslanie odpovede na bind_request
 * @param socket_id Socket
 * @param message_id ID spravy
 * @return false - pri chybe, inac true
**/
bool send_bind_response(int socket_id, int message_id)
{
    string message = create_ldap_header(message_id, BIND_RESPONSE);
    string ldap_result = create_ldap_result();
    message += create_ll(ldap_result.length());
    message += ldap_result;

    // Add 0x30, LL
    message = create_ll(message.length()) + message;
    message = hex_to_string_char(0x30) + message;

    return send_response(socket_id, message);
}

/**
 * Poslanie vsetkych result_entry sprav
 * @param socket_id Socket
 * @param message_id ID spravy
 * @param &in_database Databaza vsetkych vyfiltrovanych zaznamov
 * @param size_limit Maximalny pocet sprav na poslanie
 * @return false - pri chybe, inac true
**/
bool send_result_entry(
    int socket_id, int message_id,
    vector<vector<string>>& in_database, int size_limit
)
{
    int responses_sent = 0;
    for (auto one_vector: in_database)
    {
        if (size_limit != 0 && responses_sent == size_limit)
        {
            break;
        }

        string whole_result_entry = "";
        whole_result_entry += create_ldap_header(
            message_id, SEARCH_RESULT_ENTRY
        );
        string message = create_result_entry(one_vector);
        whole_result_entry += create_ll(message.length());
        whole_result_entry += message;

        // Add 0x30 and LL to header
        whole_result_entry = create_ll(
            whole_result_entry.length()
        ) + whole_result_entry;
        whole_result_entry = hex_to_string_char(0x30) + whole_result_entry;

        #ifdef NDEBUG
            cout << "Sending result_entry" << endl;
        #endif

        bool result = send_response(socket_id, whole_result_entry);
        responses_sent++;

        if (!result)
        {
            return false;
        }
    }

    return true;
}

/**
 * Vytvorenie result_entry spravy
 * @param input_type Vektor zaznamu z databazy
 * @return string Result_entry sprava v hex. formate
**/
string create_result_entry(vector<string>& input_type)
{
    string result_entry = "";
    result_entry += hex_to_string_char(0x04);
    string object_name = "uid=" + input_type[1];
    result_entry += create_ll(object_name.length());
    result_entry += object_name;

    // Attributes
    result_entry += hex_to_string_char(0x30);
    string partialAttributeList = create_attributelist(input_type);
    result_entry += create_ll(partialAttributeList.length());
    result_entry += partialAttributeList;

    return result_entry;
}

/**
 * Vytvorenie attributelist
 * @param input_type Vektor zaznamu z databazy
 * @return string Attributelist sprava v hex. formate
**/
string create_attributelist(vector<string>& input_type)
{
    string attribute_list = "";

    // partialAttributeList - "cn"
    attribute_list += hex_to_string_char(0x30);
    string type = create_type(input_type, "cn", 0);
    attribute_list += create_ll(type.length());
    attribute_list += type;

    // partialAttributeList - "mail"
    attribute_list += hex_to_string_char(0x30);
    type = create_type(input_type, "mail", 2);
    attribute_list += create_ll(type.length());
    attribute_list += type;

    return attribute_list;
}

/**
 * Vytvorenie type pre result_entry spravu
 * @param input_vector Vektor zaznamu z databazy
 * @param input_type Typ zaznamu z vektora
 * @param vector_index Index zaznmu z vektora
 * @return string Type v hex. formate
**/
string create_type(
    vector<string>& input_vector, string input_type, int vector_index
)
{
    string type = hex_to_string_char(0x04);
    type += create_ll(input_type.length());
    type += input_type;
    type += create_vals(input_vector, vector_index);
    return type;
}

/**
 * Vytvorenie vals pre result_entry spravu
 * @param input_vector Vektor zaznamu z databazy
 * @param vector_index Index zaznmu z vektora
 * @return string Vals v hex. formate
**/
string create_vals(vector<string>& input_vector, int vector_index)
{
    string vals = hex_to_string_char(0x31);
    string attributeValue = create_attributevalue(
        input_vector, vector_index
    );
    vals += create_ll(attributeValue.length());
    vals += attributeValue;
    return vals;
}

/**
 * Vytvorenie attributeValue pre result_entry spravu
 * @param input_vector Vektor zaznamu z databazy
 * @param vector_index Index zaznmu z vektora
 * @return string AttributeValue v hex. formate
**/
string create_attributevalue(vector<string>& input_vector, int vector_index)
{
    string attributeValue = hex_to_string_char(0x04);
    attributeValue += create_ll(input_vector[vector_index].length());
    attributeValue += input_vector[vector_index];
    return attributeValue;
}

/**
 * Poslanie result_done spravy
 * @param socket_id Socket
 * @param message_id ID spravy
 * @return false - pri chybe, inac true
**/
bool send_result_done(int socket_id, int message_id)
{
    string message = create_ldap_header(message_id, SEARCH_RESULT_DONE);
    string ldap_result = create_ldap_result();
    message += create_ll(ldap_result.length());
    message += ldap_result;

    // Add 0x30, LL
    message = create_ll(message.length()) + message;
    message = hex_to_string_char(0x30) + message;

    return send_response(socket_id, message);
}
