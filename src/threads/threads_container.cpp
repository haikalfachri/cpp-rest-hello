#include "threads_container.h"

#include <crow.h>
#include <crow/middlewares/cors.h>

#include "../globals/globals.h"
#include "../routers/router.h"

using namespace std;

void ThreadsContainer::publisher_thread() {
    publisher = new Publisher();
}

void ThreadsContainer::subscriber_thread() {
    subscriber = new Subscriber();
    subscriber->subscriber_listening();
}

void ThreadsContainer::sse_server_thread() {
    sse_server = new SSEServer();
    sse_server->start_sse();
}

void ThreadsContainer::send_to_sse_thread(string message) {
    sse_server->event_stream_handler(message);
}

void ThreadsContainer::app_server_thread() {
    crow::App<crow::CORSHandler> app;

    Router user_router(app);

    app.port(stoi(env_reader->get("APP_PORT"))).run();

    cout << "Server listening on http://" << env_reader->get("APP_URL") << ":" << env_reader->get("APP_PORT") << endl;
    cout << "SSE Server listening on http://" + env_reader->get("APP_URL") + ":" + env_reader->get("SSE_SERVER_PORT") +
                "/stream"
         << endl;
}
