/**
 * @Author Robert Kolcun (xkolcu00@stud.fit.vutbr.cz)
 * @date November, 2017
 * @brief Funkcie pre pracu s filtrami
 * 
 * Subor obsahuje funkcie pre pracu s filtrami,
 * nacitanie a spracovanie filtrov. Filtrovanie zaznamov
 * zo vstupneho suboru. Pomocne funkcie pre debugovanie.
**/

#include "ldapfilter.h"

using namespace std;

/**
 * Podporovane attributeDesc
 * Jméno (cn, CommonName)
 * Login (uid, UserID)
 * Email (mail)
**/
set<string> known_attrdesc({
    "cn", "commonname", "uid", "userid", "mail"
});

/**
 * Konstuktor pre triedu Filter
 * @param input_ll Dlzka filtra
 * @param input_type Typ filtra
**/
Filter::Filter (int input_ll, unsigned char input_type)
{
    ll = input_ll;
    type = input_type;
    is_null = false;
}

/**
 * Konstruktor pre chybny filter
 * @param input_null Boolean chybova hodnota
**/
Filter::Filter (bool input_null)
{
    ll = 0;
    type = 0x00;
    is_null = input_null;
}

/**
 * Pomocna funkcie pre rekurzivne vypisanie
 * nacitanych filtrov
 * @param input_filter Korenovy filter
 * @param spaces Pocet medzier pre vypis
**/
void print_filter(Filter input_filter, int spaces)
{
    for (int i = 0; i < spaces; ++i)
    {
        fprintf(stderr, " ");
    }
    fprintf(stderr, "LL:%d ", input_filter.ll);
    print_filter_type(input_filter.type);

    spaces += 3;

    for (auto i: input_filter.filter_vector)
    {
        print_filter(i, spaces);
    }
}

/**
 * Pomocna funkcie pre vypis typu filtra
 * @param filter_type Typ filtra
**/
void print_filter_type(unsigned char filter_type)
{
    switch(filter_type)
    {
        case FILTER_AND:
            fprintf(stderr, "FILTER: %s\n", "AND");
            break;
        case FILTER_OR:
            fprintf(stderr, "FILTER: %s\n", "OR");
            break;
        case FILTER_NOT:
            fprintf(stderr, "FILTER: %s\n", "NOT");
            break;
        case FILTER_SUBSTRING:
            fprintf(stderr, "FILTER: %s\n", "SUB_STRING");
            break;
        case FILTER_EQUALITYMATCH:
            fprintf(stderr, "FILTER: %s\n", "EQUALITY_MATCH");
            break;
        case FILTER_END:
            fprintf(stderr, "FILTER: %s\n", "END");
            break;
    }
}

/**
 * Funkcia vrati typ filtra
 * @param socket_id Socket
 * @return FILTER_ERROR pri chybe, inac typ filtra
**/
unsigned char get_filter_type(int socket_id)
{
    unsigned char answer = read_char(socket_id);
    switch(answer)
    {
        case FILTER_AND:
        case FILTER_OR:
        case FILTER_NOT:
        case FILTER_SUBSTRING:
        case FILTER_EQUALITYMATCH:
        case FILTER_END:
            return answer;
        default:
            return FILTER_ERROR;
    }
}

/**
 * Vstupna funkcie pre nacitanie filtrov
 * @param socket_id Socket
 * @return Filter class Korenovy filter
**/
Filter process_filters(int socket_id)
{
    return load_filter(socket_id);
}

/**
 * Funkcie pre rekurzivne nacitanie filtrov
 * @param socket_id Socket
 * @return Filter class Nacitany filter
**/
Filter load_filter(int socket_id)
{
#ifdef NDEBUG
    fprintf(stderr, "%s\n", "Loading new filter...");
#endif

    unsigned char filter_type = get_filter_type(socket_id);
    if (filter_type == FILTER_ERROR)
    {
        print_stderr_message("Invalid filter");
        return Filter(true);
    }

    int filter_ll = read_char(socket_id);
    Filter new_filter = Filter(
        filter_ll, filter_type
    );

    int actual_ll = 0;

    while (filter_type != FILTER_SUBSTRING && filter_type != FILTER_EQUALITYMATCH)
    {
        Filter return_filter = load_filter(socket_id);

#ifdef NDEBUG
    fprintf(stderr, "%s\n", "Filter returned...");
    print_filter_type(filter_type);
#endif

        // Chyba pri nacitani filtra
        if (return_filter.is_null)
        {
            return return_filter;
        }

        // Pridanie filtra do vectoru a skontrolovanie dlzky
        new_filter.filter_vector.push_back(return_filter);
        actual_ll += return_filter.ll + 2;
        if (actual_ll == filter_ll)
        {
#ifdef NDEBUG
    fprintf(stderr, "%s\n", "LL returning...");
#endif
            return new_filter;
        }
    }

    // Spracovanie SUBSTRING a EUQLITYMATCH filtra
    switch(filter_type)
    {
        case FILTER_EQUALITYMATCH:
            equalitymatch_attributes(
                socket_id, new_filter
            );
            break;
        case FILTER_SUBSTRING:
            substring_add_attributes(
                socket_id, new_filter
            );
            break;
        default:
            print_stderr_message("Invalid filter");
            new_filter.is_null = true;
            break;
    }

    return new_filter;
}

/**
 * Pridanie atributov do equalitymatch filtra
 * @param socket_id Socket
 * @param &input_filter Equlitymatch filter
**/
void equalitymatch_attributes(int socket_id, Filter& input_filter)
{
#ifdef NDEBUG
    fprintf(stderr, "START: %s\n", "Equality match!");
#endif

    // 0x04, LL, abcd... - attributeDesc sequence
    unsigned char answer = read_char(socket_id);
    if (answer != 0x04)
    {
        print_stderr_message("Invalid 0x04 bye in equlitymatch_filter");
        input_filter.is_null = true;
        return;
    }
    int message_ll = read_LL(socket_id);
    string attributeDesc = lower_string(read_message(socket_id, message_ll));

    // Check if attribute exists
    set<string>::iterator it = known_attrdesc.find(attributeDesc);
    if (it == known_attrdesc.end())
    {
        print_stderr_message("Invalid attributeDesc in equalitymatch_filter");
        input_filter.is_null = true;
        return;
    }

    // 0x04, LL, abcd... - assertionValue sequence
    answer = read_char(socket_id);
    if (answer != 0x04)
    {
        print_stderr_message("Invalid 0x04 bye in equlitymatch_filter");
        input_filter.is_null = true;
        return;
    }
    message_ll = read_LL(socket_id);
    string assertionValue = read_message(socket_id, message_ll);

    // Pridanie atributu do filtra
    input_filter.attr[attributeDesc] = assertionValue;
}

/**
 * Pridanie atributov do substring filtra
 * @param socket_id Socket
 * @param &input_filter Substring filter
**/
void substring_add_attributes(int socket_id, Filter& input_filter)
{
#ifdef NDEBUG
    fprintf(stderr, "START: %s\n", "Substring filter!");
#endif

    // 0x04, LL, abcd... - type
    unsigned char answer = read_char(socket_id);
    if (answer != 0x04)
    {
        print_stderr_message("Invalid 0x04 byte in substring_filter");
        input_filter.is_null = true;
        return;
    }
    int type_ll = read_LL(socket_id);
    string type = lower_string(read_message(socket_id, type_ll));

    // Kontrola ci vstupny type existuje
    set<string>::iterator it = known_attrdesc.find(type);
    if (it == known_attrdesc.end())
    {
        print_stderr_message("Invalid type in substring_filter");
        input_filter.is_null = true;
        return;
    }

    // 0x30, LL - substrings
    answer = read_char(socket_id);
    if (answer != 0x30)
    {
        print_stderr_message("Invalid 0x30 byte in substring_filter");
        input_filter.is_null = true;
        return;
    }
    int substring_ll = read_LL(socket_id);
    string substring_value = "";
    int actual_ll = 0;

    // Spracovanie celeho substring filtra
    unsigned char substring_type = read_char(socket_id);
    switch(substring_type)
    {
        case SUBSTRING_INITIAL:
            process_substring_initial(
                socket_id, substring_value, actual_ll, substring_ll
            );
            break;
        case SUBSTRING_ANY:
            process_substring_any(
                socket_id, substring_value, actual_ll, substring_ll
            );
            break;
        case SUBSTRING_FINAL:
            process_substring_final(
                socket_id, substring_value, actual_ll, substring_ll
            );
            break;
        default:
            print_stderr_message("Invalid substring type");
            input_filter.is_null = true;
            return;
    }

#ifdef NDEBUG
    fprintf(stderr, "Substring: %s\n", substring_value.c_str());
    fprintf(stderr, "Substr length: %d\n", actual_ll);
#endif

    // Kontrola dlzky
    if (substring_ll != actual_ll)
    {
        print_stderr_message("Invalid substring length");
        input_filter.is_null = true;
        return;
    }

    // Pri chybe je vystupny retazec prazdny
    if (!strcmp(substring_value.c_str(), ""))
    {
        print_stderr_message("Invalid substring_value");
        input_filter.is_null = true;
        return;
    }

    // Pridanie atributu do filtra
    input_filter.attr[type] = substring_value;
}

/**
 * Spracovanie substring initial filtra
 * @param socket_id Socket
 * @param &input_string Retazec spracovaneho substring filtra
 * @param &actual_ll Aktualna dlzka nacitaneho filtra
 * @param substring_ll Celkova dlzka filtra
**/
void process_substring_initial(
    int socket_id, string& input_string, int& actual_ll, int substring_ll
)
{
    // LL, abcd... - substring
    int message_ll = read_LL(socket_id);
    string substring_message = read_message(socket_id, message_ll);
    input_string += (substring_message + ".*");
    actual_ll += (message_ll + 2);

    if (actual_ll == substring_ll)
    {
        return;
    }

    unsigned char substring_type = read_char(socket_id);
    switch(substring_type)
    {
        case SUBSTRING_ANY:
            process_substring_any(
                socket_id, input_string, actual_ll, substring_ll
            );
            break;
        case SUBSTRING_FINAL:
            process_substring_final(
                socket_id, input_string, actual_ll, substring_ll
            );
            break;
        default:
            print_stderr_message("Invalid substring type in substring_initial");
            input_string = "";
            break;
    }
}

/**
 * Spracovanie substring any filtra
 * @param socket_id Socket
 * @param &input_string Retazec spracovaneho substring filtra
 * @param &actual_ll Aktualna dlzka nacitaneho filtra
 * @param substring_ll Celkova dlzka filtra
**/
void process_substring_any(
    int socket_id, string& input_string, int& actual_ll, int substring_ll
)
{
    // LL, abcd... - substring
    int message_ll = read_LL(socket_id);
    string substring_message = read_message(socket_id, message_ll);

    // Check (.*)
    input_string += (".*" + substring_message + ".*");
    actual_ll += (message_ll + 2);

    if (actual_ll == substring_ll)
    {
        return;
    }

    unsigned char substring_type = read_char(socket_id);
    switch(substring_type)
    {
        case SUBSTRING_ANY:
            process_substring_any(
                socket_id, input_string, actual_ll, substring_ll
            );
            break;
        case SUBSTRING_FINAL:
            process_substring_final(
                socket_id, input_string, actual_ll, substring_ll
            );
            break;
        default:
            print_stderr_message("Invalid substring type in substring_any");
            input_string = "";
            break;
    }
}

/**
 * Spracovanie substring final filtra
 * @param socket_id Socket
 * @param &input_string Retazec spracovaneho substring filtra
 * @param &actual_ll Aktualna dlzka nacitaneho filtra
 * @param substring_ll Celkova dlzka filtra
**/
void process_substring_final(
    int socket_id, string& input_string, int& actual_ll, int substring_ll
)
{
    // LL, abcd... - substring
    int message_ll = read_LL(socket_id);
    string substring_message = read_message(socket_id, message_ll);
    input_string += (".*" + substring_message);
    actual_ll += (message_ll + 2);

    if (actual_ll == substring_ll)
    {
        return;
    }
    else
    {
        print_stderr_message("Invalid filter end in substring_final");
        input_string = "";
    }
}

/**
 * Funkcie pre vyhladanie a zaznamov na zaklade vstupneho filtra
 * @param root_filter Korenovy filter
 * @param input_database Vstupna databaze zaznamov
 * @return vector<vector<string>> Vektor vyfiltrovanych zaznamov
**/
vector<vector<string>> get_database_vector(
    Filter root_filter, vector<vector<string>> input_database
)
{
#ifdef NDEBUG
    fprintf(stderr, "Get_vector: ");
    print_filter_type(root_filter.type);
#endif
    switch(root_filter.type)
    {
        case FILTER_AND:
            return process_filter_and(
                root_filter.filter_vector, input_database
            );
        case FILTER_OR:
            return process_filter_or(
                root_filter.filter_vector, input_database
            );
        case FILTER_NOT:
            return process_filter_not(
                root_filter.filter_vector.front(), input_database
            );
        case FILTER_SUBSTRING:
            return process_filter_substring(
                root_filter, input_database
            );
        case FILTER_EQUALITYMATCH:
            return process_filter_equalitymatch(
                root_filter, input_database
            );
        default:
            print_stderr_message("Invalid filter type in applyFilter");
            return vector<vector<string>>();
    }
}

/**
 * Spracovanie AND filtra
 * @param in_filter_vec Vstupny vector filtrov
 * @param in_database Vstupna databaza
 * @return vector<vector<string>> Vektor vyfiltrovanych zaznamov
**/
vector<vector<string>> process_filter_and(
    vector<Filter> in_filter_vec, vector<vector<string>> in_database
)
{
    vector<vector<string>> new_data = in_database;
    for (auto one_filter: in_filter_vec)
    {
        new_data = get_database_vector(one_filter, new_data);
    }
    return new_data;
}

/**
 * Spracovanie OR filtra
 * @param in_filter_vec Vstupny vector filtrov
 * @param in_database Vstupna databaza
 * @return vector<vector<string>> Vektor vyfiltrovanych zaznamov
**/
vector<vector<string>> process_filter_or(
    vector<Filter> in_filter_vec, vector<vector<string>> in_database
)
{
    vector<vector<string>> new_data;
    for (auto one_filter: in_filter_vec)
    {
        union_vectors(
            new_data, get_database_vector(one_filter, in_database)
        );
    }
    return new_data;
}

/**
 * Spracovanie NOT filtra
 * @param in_filter Vstupny filter
 * @param in_database Vstupna databaza
 * @return vector<vector<string>> Vektor vyfiltrovanych zaznamov
**/
vector<vector<string>> process_filter_not(
    Filter in_filter, vector<vector<string>> in_database
)
{
    vector<vector<string>> filtered_data;
    vector<vector<string>> not_data = get_database_vector(
        in_filter, in_database
    );

    for (auto i: in_database)
    {
        if (!is_vector_in(i, not_data))
        {
            filtered_data.push_back(i);
        }
    }

    return filtered_data;
}

/**
 * Spracovanie SUBSTRING filtra
 * @param in_filter Vstupny filter
 * @param in_database Vstupna databaza
 * @return vector<vector<string>> Vektor vyfiltrovanych zaznamov
**/
vector<vector<string>> process_filter_substring(
    Filter in_filter, vector<vector<string>> in_database
)
{
    string attr_name = lower_string(in_filter.attr.begin()->first);
    string attr_value = lower_string(in_filter.attr.begin()->second);

#ifdef NDEBUG
    cout << attr_name << ":" << attr_value << endl;
#endif

    vector<vector<string>> new_data;
    for (auto i: in_database)
    {
        if (!strcmp(attr_name.c_str(), "cn") || !strcmp(attr_name.c_str(), "commonname"))
        {
            if (regex_match(lower_string(i[0]), regex(attr_value)))
            {
                new_data.push_back(i);
            }
            continue;
        }

        if (!strcmp(attr_name.c_str(), "uid") || !strcmp(attr_name.c_str(), "userid"))
        {
            if (regex_match(lower_string(i[1]), regex(attr_value)))
            {
                new_data.push_back(i);
            }
            continue;
        }

        if (!strcmp(attr_name.c_str(), "mail"))
        {
            if (regex_match(lower_string(i[2]), regex(attr_value)))
            {
                new_data.push_back(i);
            }
            continue;
        }
    }

    return new_data;
}

/**
 * Spracovanie EQUALITY_MATCH filtra
 * @param in_filter Vstupny filter
 * @param in_database Vstupna databaza
 * @return vector<vector<string>> Vektor vyfiltrovanych zaznamov
**/
vector<vector<string>> process_filter_equalitymatch(
    Filter in_filter, vector<vector<string>> in_database
)
{
    string attr_name = lower_string(in_filter.attr.begin()->first);
    string attr_value = lower_string(in_filter.attr.begin()->second);

#ifdef NDEBUG
    cout << attr_name << ":" << attr_value << endl;
#endif

    vector<vector<string>> new_data;
    for (auto i: in_database)
    {
        if (!strcmp(attr_name.c_str(), "cn") || !strcmp(attr_name.c_str(), "commonname"))
        {
            if (!strcmp(attr_value.c_str(), lower_string(i[0]).c_str()))
            {
                new_data.push_back(i);
            }
            continue;
        }

        if (!strcmp(attr_name.c_str(), "uid") || !strcmp(attr_name.c_str(), "userid"))
        {
            if (!strcmp(attr_value.c_str(), lower_string(i[1]).c_str()))
            {
                new_data.push_back(i);
            }
            continue;
        }

        if (!strcmp(attr_name.c_str(), "mail"))
        {
            if (!strcmp(attr_value.c_str(), lower_string(i[2]).c_str()))
            {
                new_data.push_back(i);
            }
            continue;
        }
    }

    return new_data;
}

/**
 * Zjednotenie obydvoch vstupnych databaz do prvej
 * @param &self_data Vstupno/Vypstupna databaza zaznamov
 * @param other_data Vstupna databaza zaznamov
**/
void union_vectors(
    vector<vector<string>>& self_data, vector<vector<string>> other_data
)
{
    // Do self_data pridat vsetky ktore tam niesu z other_data
    for (auto i: other_data)
    {
        if (!is_vector_in(i, self_data))
        {
            self_data.push_back(i);
        }
    }
}

/**
 * Kontrola ci filter sa nachadza v databaze
 * @param check_vector Kontrolny vektor ktory (ne)nechadza v databaze
 * @param group_data Kontrolna databaza
 * @return true ak sa vektor nachadza v databaze, inac false
**/
bool is_vector_in(vector<string>& check_vector, vector<vector<string>>& group_data)
{
    for (auto i: group_data)
    {
        if (!strcmp(i[0].c_str(), check_vector[0].c_str()))
        {
            if (!strcmp(i[1].c_str(), check_vector[1].c_str()))
            {
                if (!strcmp(i[2].c_str(), check_vector[2].c_str()))
                {
                    return true;
                }
            }
        }
    }
    return false;
}
