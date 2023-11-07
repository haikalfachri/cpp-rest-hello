#include "../controllers/user_controller/user_controller.h"
#include "router.h"

Router::Router(crow::App<crow::CORSHandler>& app) {
    this->set_routes(app);
}

void Router::set_routes(crow::App<crow::CORSHandler>& app) {
    CROW_ROUTE(app, "/users")
        .methods(crow::HTTPMethod::POST)([&](const crow::request& kReq, crow::response& res) {
            UserController::create_user(kReq, res);
        });

    CROW_ROUTE(app, "/users")
        .methods(crow::HTTPMethod::GET)([&](const crow::request& kReq, crow::response& res) {
            UserController::get_all_users(kReq, res);
        });
}