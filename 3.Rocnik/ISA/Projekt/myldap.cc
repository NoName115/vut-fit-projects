/**
 * @Author Robert Kolcun (xkolcu00@stud.fit.vutbr.cz)
 * @date November, 2017
 * @brief Spustenie servera a hlavne spracovanie poziadavkov
 *
 * Subor obsahuje main() funkciu, kontrolu vstupnych argumentov,
 * spustenie vlakien pre kazdeho klienta,
 * hlavnu while slucku a spracovanie poziadavok od clienta.
**/

#include "responser.h"

using namespace std;

/**
 * Spracovanie bind_request-u a poslanie bind_response
 * @param socket_id Socket
 * @param message_id ID spravy
 * @return true - vsetko prebehlo v poriadku, inac false - nastala chyba
**/
bool process_bind_request(int socket_id, int message_id)
{
#ifdef NDEBUG
    fprintf(stderr, "\nSTART: %s\n", "Bind request!");
#endif

    int request_LL = read_LL(socket_id);

    // 0x02, 0x01 sequence
    unsigned char answer = read_char(socket_id);
    if (answer != 0x02)
    {
        print_stderr_message("Invalid 0x02 byte in bind_request");
        return false;
    }

    answer = read_char(socket_id);
    if (answer != 0x01)
    {
        print_stderr_message("Invalid 0x01 byte in bind_request");
        return false;
    }

    // Verzia protokolu - 0x02[1..127]
    answer = read_char(socket_id);
    /*
    if (answer != 0x02)
    {
        print_stderr_message("Invalid 0x02 protocol version in bind_request");
        return false;
    }
    */

    // 0x04, LL, abcd... sequence
    answer = read_char(socket_id);
    if (answer != 0x04)
    {
        print_stderr_message("Invalid 0x04 byte in bind_request");
        return false;
    }

    int name_LL = read_LL(socket_id);
    string name_message = read_message(socket_id, name_LL);

    // 0x80, LL, abcd... sequence
    answer = read_char(socket_id);
    if (answer != 0x80)
    {
        print_stderr_message("Invalid 0x80 byte in bind_request");
        return false;
    }

    int simple_LL = read_LL(socket_id);
    string simple_message = read_message(socket_id, simple_LL);

    // Kontrola dlzky spravy
    int actual_length = 5 + 2 + name_LL + simple_LL;
    if (request_LL != actual_length)
    {
        if (request_LL == (actual_length + 1))
        {
            unsigned char end_char = read_char(socket_id);
            if (end_char != 0xA0)
            {
                print_stderr_message("Invalid 0xA0 end byte in bind_request");
                return false;
            }
        }
        else
        {
            print_stderr_message("Invalid messange length in bind_request");
            return false;
        }
    }

#ifdef NDEBUG
    fprintf(stderr, "END: %s\n", "Bind request!");
    fprintf(stderr, "\nSTART: %s\n", "Bind response!");
#endif

    bool result = send_bind_response(socket_id, message_id);

#ifdef NDEBUG
    fprintf(stderr, "END: %s\n", "Bind response!");
#endif

    return result;
}

/**
 * Spracovanie search_request-u, filtrov, vyhladanie zaznamov v databaze
 * odoslanie searchResultEntry a searchResultDone
 * @param socket_id Socket
 * @param message_id ID spravy
 * @param input_database Cela databaza zo vstupneho suboru
 * @return true - vsetko prebehlo v poriadku, inac false - nastala chyba
**/
bool process_search_request(
    int socket_id, int message_id, vector<vector<string>> input_database
)
{
#ifdef NDEBUG
    fprintf(stderr, "\nSTART: %s\n", "Search request!");
#endif

    int request_LL = read_LL(socket_id);

    // 0x04, LL, message - baseObject
    unsigned char answer = read_char(socket_id);
    if (answer != 0x04)
    {
        print_stderr_message("Invalid 0x04 start byte in search_request");
        return false;
    }
    int message_LL = read_LL(socket_id);
    string message = read_message(socket_id, message_LL);

    // 0x0A, 0x01, [0, 1, 2] - scope
    answer = read_char(socket_id);
    if (answer != 0x0A)
    {
        print_stderr_message("Invalid 0x0A byte in search_request");
        return false;
    }
    answer = read_char(socket_id);
    if (answer != 0x01)
    {
        print_stderr_message("Invalid 0x01 byte in search_request");
        return false;
    }
    answer = read_char(socket_id);
    if (answer != 0x00 && answer != 0x01 && answer != 0x02)
    {
        print_stderr_message("Invalid byte[0x00, 0x01, 0x02] in search_request");
        return false;
    }

    // 0x0A, 0x01, [0..3] - derefAliases
    answer = read_char(socket_id);
    if (answer != 0x0A)
    {
        print_stderr_message("Invalid byte 0x0A in search_request");
        return false;
    }
    answer = read_char(socket_id);
    if (answer != 0x01)
    {
        print_stderr_message("Invalid byte 0x01 in search_request");
        return false;
    }
    answer = read_char(socket_id);
    if (answer > 0x03)
    {
        print_stderr_message("Invalid byte[0..3] in search_request");
        return false;
    }

    // 0x02, 0x0[1..4][0, 2^31-1] - sizeLimit
    answer = read_char(socket_id);
    if (answer != 0x02)
    {
        print_stderr_message("Invalid byte 0x02 in search_request");
        return false;
    }
    int size_limit = read_message_id(socket_id);
    if (size_limit < 0)
    {
        print_stderr_message("Invalid sizeLimit, is lower than 0 in search_request");
        return false;
    }

#ifdef NDEBUG
    fprintf(stderr, "SizeLimit: %d\n", size_limit);
#endif

    // 0x02, 0x0[1..4][0, 2^31-1] - timeLimit
    answer = read_char(socket_id);
    if (answer != 0x02)
    {
        print_stderr_message("Invalid byte 0x02 in search_request");
        return false;
    }
    int timeLimit = read_message_id(socket_id);
    if (timeLimit < 0)
    {
        print_stderr_message("Invalid timeLimit, is lower than 0 in search_request");
        return false;
    }

    // 0x01, 0x01 , bool - types only
    answer = read_char(socket_id);
    if (answer != 0x01)
    {
        print_stderr_message("Invalid byte 0x01 in search_request");
        return false;
    }
    answer = read_char(socket_id);
    if (answer != 0x01)
    {
        print_stderr_message("Invalid byte 0x01 in search_request");
        return false;
    }
    answer = read_char(socket_id);
    if (answer != 0x00 && answer != 0x01)
    {
        print_stderr_message("Invalid byte[0x00, 0x01] in search_request");
        return false;
    }

    // --------- FILTERS ----------
    Filter return_filter = process_filters(socket_id);
    if (return_filter.is_null)
    {
        print_stderr_message("Return filter is null in search_request");
        return false;
    }

    // 0x30 0x00
    answer = read_char(socket_id);
    if (answer != 0x30)
    {
        print_stderr_message("Invalid byte 0x30 in search_request");
        return false;
    }
    answer = read_char(socket_id);
    if (answer != 0x00)
    {
        print_stderr_message("Invalid byte 0x00 in search_request");
        return false;
    }

#ifdef NDEBUG
    fprintf(stderr, "\n----- FILTERS -----\n");
    print_filter(return_filter, 0);
#endif

    vector<vector<string>> filtered_database = get_database_vector(
        return_filter, input_database
    );

#ifdef NDEBUG
    for (auto i: filtered_database)
    {
        cout << i[0] << " " << i[1] << " " << i[2] << endl;
    }
#endif

    // Poslat SearchResultEntry
    bool result = send_result_entry(
        socket_id, message_id, filtered_database, size_limit
    );
    if (!result)
    {
        return false;
    }

    // Poslat SearchResultDone
    result = send_result_done(socket_id, message_id);
    if (!result)
    {
        return false;
    }

#ifdef NDEBUG
    fprintf(stderr, "END: %s\n", "Search request!");
#endif

    return true;
}

bool process_unbind_request(int socket_id, int message_id)
{
#ifdef NDEBUG
    fprintf(stderr, "\nSTART: %s\n", "Unbind request!");
#endif

#ifdef NDEBUG
    fprintf(stderr, "END: %s\n", "Unbind request!");
#endif
}

/**
 * Spracovanie hlavicky LDAP_message a urcenie typu poziadavku
 * @param socket_id Socket
 * @param input_data Vstupna databaza zaznamov zo suboru
 * @return true - vsetko prebehlo v poriadku, inac false - nastala chyba
**/
bool parse_message(int socket_id, vector<vector<string>> input_data)
{
    // LDAP message
    // Check first 0x30
    unsigned char answer = read_char(socket_id);
    if (answer != 0x30)
    {
        print_stderr_message("Invalid 0x30 start byte");
        return false;
    }

    // Read LL
    int message_ll = read_LL(socket_id);

    // Check 0x02 before message_id
    answer = read_char(socket_id);
    if (answer != 0x02)
    {
        print_stderr_message("Invalid 0x02 before message_id");
        return false;
    }

    int message_id = read_message_id(socket_id);
    if (message_id < 0)
    {
        print_stderr_message("Invalid message_id, lower than 0");
        return false;
    }

    // Request type
    bool is_message_valid = true;
    unsigned char message_type = read_char(socket_id);
    switch(message_type)
    {
        case BIND_REQUEST:
            is_message_valid = process_bind_request(
                socket_id, message_id
            );
            break;
        case SEARCH_REQUEST:
            is_message_valid = process_search_request(
                socket_id, message_id, input_data
            );
            break;
        case UNBIND_REQUEST:
            is_message_valid = process_unbind_request(
                socket_id, message_id
            );
            return false;
        default:
            print_stderr_message("Unknown request!!");
            return false;
    }

    return is_message_valid;
}

/**
 * Hlavny cyklus while pre spracovanie sprav a vyhodnotenie pokracovania
 * @param commSocket Socket
 * @param input_data Vstupna databaza zaznamov zo suboru
**/
void process_client(int commSocket, vector<vector<string>> input_data)
{
#ifdef NDEBUG
    fprintf(stderr, "%s\n", "New client connected!");
#endif

    bool process = true;
    while (process)
    {
        process = parse_message(
            commSocket, input_data
        );
    }

    close(commSocket);

#ifdef NDEBUG
    fprintf(stderr, "%s\n", "Cliend ending...");
#endif
}

/**
 * Inicializovanie servera, kontrola argumentov, nacitanie
 * dat zo vstupneho subora, zachytenie poziadavky klienta
 * a vytvorenie samostaneho vlakna pre noveho klienta
 * @param argc Pocet vstupnych argumentov
 * @param **argv Vstupne argumenty
 * @return -1 pri chybe, inac 0
**/
int main(int argc, char **argv)
{
    struct sockaddr_in serverAddr;
    string hostAdress = "localhost";
    int portNumber = 389; //389
    string file_path = "";

    // --------- ARGUMENTS ---------
    // Check input arguments
    if (argc != 3 && argc != 5)
    {
        fprintf(stderr, "%s\n", "Invalid input argument");
        return -1;
    }

    // Parse input arguments
    for (int i = 1; i < argc; i += 2)
    {
        if (!strcmp(argv[i], "-p"))
        {
            try
            {
                portNumber = stoi(argv[i + 1]);
            }
            catch (std::invalid_argument& e)
            {
                fprintf(stderr, "%s\n", "Invalid input argument");
                return -1;
            }
        }
        else if (!strcmp(argv[i], "-f"))
        {
            file_path += argv[i + 1];
        }
        else
        {
            fprintf(stderr, "%s\n", "Invalid input argument");
            return -1;
        }
    }

    if (!strcmp(file_path.c_str(), ""))
    {
        fprintf(stderr, "%s\n", "Invalid input argument");
        return -1;
    }

#ifdef NDEBUG
    printf("Filepath: %s\n", file_path.c_str());
    printf("Port: %d\n", portNumber);
#endif

    // --------- DATABASE LOAD ----------
    // Check file and load it
    ifstream input_file(file_path);
    if (!input_file.is_open())
    {
        fprintf(stderr, "%s\n", "Cant open input file");
        return -1;
    }

    vector<vector<string>> database;

    // Read file
    string one_line;
    while (getline(input_file, one_line, ';'))
    {
        vector<string> line_record;

        // Name
        line_record.push_back(one_line);
        // Login
        getline(input_file, one_line, ';');
        line_record.push_back(one_line);
        // Email
        getline(input_file, one_line);
        string r_removed = one_line.substr(0, one_line.length() - 1);
        line_record.push_back(r_removed);

        database.push_back(line_record);
    }

    // --------- CONNECTION ----------
    // Init socket
    int welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (welcomeSocket < 0)
    {
        fprintf(stderr, "%s\n", "Error establishing connection.");
        return -1;
    }

#ifdef NDEBUG
    fprintf(stderr, "%s\n", "Server socket connection created...");
#endif

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(portNumber);

    // Binding socket
    if (bind(welcomeSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        fprintf(stderr, "%s\n", "Error binding socket");
        return -1;
    }

#ifdef NDEBUG
    fprintf(stderr, "%s\n", "Looking for clients...");
#endif

    // Listening socket
    if (listen(welcomeSocket, 1) < 0)
    {
        fprintf(stderr, "%s\n", "Error listening socket");
        return -1;
    }

    socklen_t size = sizeof(serverAddr);
    while (1)
    {
        int commSocket = accept(welcomeSocket, (struct sockaddr*)&serverAddr, &size);
        if (commSocket > 0)
        {
            // Create client thread
#ifdef NDEBUG
    fprintf(stderr, "%s\n", "Client connected");
#endif
            thread(process_client, commSocket, database).detach();
        }
    }

    return 0;
}
