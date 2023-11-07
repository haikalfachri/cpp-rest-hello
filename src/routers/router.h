#pragma once

#include <crow.h>
#include <crow/middlewares/cors.h>

class Router {
  public:
    Router(crow::App<crow::CORSHandler>& app);

  private:
    void set_routes(crow::App<crow::CORSHandler>& app);
};