#pragma once

#include <crow.h>

class UserController {
  public:
    UserController();

    static void get_all_users(const crow::request& kReq, crow::response& res);

    static void create_user(const crow::request& kReq, crow::response& res);
};