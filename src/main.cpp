#include <crow.h>
#include <crow/middlewares/cors.h>

#include <iostream>
#include <thread>

#include "./globals/globals.h"
#include "./threads/threads_container.h"

using namespace std;

/* Global variables and objects definition */
Database *database;
Publisher *publisher;
Subscriber *subscriber;
SSEServer *sse_server;
EnvReader *env_reader;
/* End of definition */

void RunApp() {
    try {
        env_reader = new EnvReader("../../.env");

        database = new Database();
        pqxx::connection &db_connection = database->get_connection();
        cout << "Data Source Name (DSN): " << db_connection.connection_string() << endl;
        database->create_table();

        thread app_server_thread(ThreadsContainer::app_server_thread);
        thread publisher_thread(ThreadsContainer::publisher_thread);
        thread subscriber_thread(ThreadsContainer::subscriber_thread);
        thread sse_server_thread(ThreadsContainer::sse_server_thread);

        app_server_thread.join();
        publisher_thread.join();
        subscriber_thread.join();
        sse_server_thread.join();

    } catch (const exception &e) {
        cerr << "Exception caught: " << e.what() << endl;
    }
}

int main(int argc, char **argv) {
    RunApp();
    return 0;
}
