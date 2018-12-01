/**
 * @Author Robert Kolcun (xkolcu00@stud.fit.vutbr.cz)
 * @date November, 2017
 * @brief Hlavickovy subor pre responser.cc
**/

#include "ldapfilter.h"

#define BIND_REQUEST 0x60
#define BIND_RESPONSE 0x61
#define UNBIND_REQUEST 0x42
#define SEARCH_REQUEST 0x63
#define SEARCH_RESULT_ENTRY 0x64
#define SEARCH_RESULT_DONE 0x65

std::string create_ldap_header(
    int message_id,
    unsigned char protocol
);
std::string create_ldap_result();
bool send_response(int socket_id, std::string message);

bool send_bind_response(int socket_id, int message_id);
bool send_result_entry(
    int socket_id,
    int message_id,
    std::vector<std::vector<std::string>>& in_database,
    int size_limit
);
bool send_result_done(int socket_id, int message_id);

std::string create_result_entry(
    std::vector<std::string>& input_type
);
std::string create_attributelist(
    std::vector<std::string>& input_type
);
std::string create_attributevalue(
    std::vector<std::string>& input_vector,
    int vector_index
);
std::string create_vals(
    std::vector<std::string>& input_vector,
    int vector_index
);
std::string create_type(
    std::vector<std::string>& input_vector,
    std::string input_type,
    int vector_index
);
std::string create_attributelist(
    std::vector<std::vector<std::string>>& in_database
);
