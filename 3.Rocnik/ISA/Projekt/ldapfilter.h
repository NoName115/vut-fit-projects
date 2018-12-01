/**
 * @Author Robert Kolcun (xkolcu00@stud.fit.vutbr.cz)
 * @date November, 2017
 * @brief Hlavickovy subor pre ldapsearch.cc
**/

#include "basefunc.h"

#define FILTER_AND 0xA0
#define FILTER_OR 0xA1
#define FILTER_NOT 0xA2
#define FILTER_SUBSTRING 0xA4
#define FILTER_EQUALITYMATCH 0xA3
#define FILTER_END 0x30
#define FILTER_ERROR 0xFF

#define SUBSTRING_INITIAL 0x80
#define SUBSTRING_ANY 0x81
#define SUBSTRING_FINAL 0x82

class Filter {
  public:
    int ll;
    unsigned char type;
    std::vector<Filter> filter_vector;
    bool is_null;
    std::map<std::string, std::string> attr;

  public:
    Filter (int input_ll, unsigned char input_type);
    Filter (bool input_null);
};

void print_filter_type(unsigned char filter_type);
void print_filter(Filter input_filter, int spaces);

Filter process_filters(int socket_id);
Filter load_filter(int socket_id);
void equalitymatch_attributes(int socket_id, Filter& input_filter);
void substring_add_attributes(int socket_id, Filter& input_filter);

void process_substring_initial(
    int socket_id,
    std::string& input_string,
    int& actual_ll,
    int substring_ll
);
void process_substring_any(
    int socket_id,
    std::string& input_string,
    int& actual_ll,
    int substring_ll
);
void process_substring_final(
    int socket_id,
    std::string& input_string,
    int& actual_ll,
    int substring_ll
);

void union_vectors(
    std::vector<std::vector<std::string>>& self_data,
    std::vector<std::vector<std::string>> other_data
);
bool is_vector_in(
    std::vector<std::string>& check_vector,
    std::vector<std::vector<std::string>>& group_data
);

std::vector<std::vector<std::string>> get_database_vector(
    Filter root_filter,
    std::vector<std::vector<std::string>> input_database
);

std::vector<std::vector<std::string>> process_filter_and(
    std::vector<Filter> in_filter_vec,
    std::vector<std::vector<std::string>> in_database
);
std::vector<std::vector<std::string>> process_filter_or(
    std::vector<Filter> in_filter_vec,
    std::vector<std::vector<std::string>> in_database
);
std::vector<std::vector<std::string>> process_filter_not(
    Filter in_filter,
    std::vector<std::vector<std::string>> in_database
);
std::vector<std::vector<std::string>> process_filter_substring(
    Filter in_filter,
    std::vector<std::vector<std::string>> in_database
);
std::vector<std::vector<std::string>> process_filter_equalitymatch(
    Filter in_filter,
    std::vector<std::vector<std::string>> in_database
);
