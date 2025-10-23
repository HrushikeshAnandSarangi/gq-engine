#pragma once
#include <drogon/HttpSimpleController.h>

using namespace drogon;

class MarketController: public drogon::HttpSimpleController<MarketController>
{
  public:
    PATH_LIST_BEGIN
    PATH_ADD("/orders", Get, Post);
    PATH_LIST_END

    void asyncHandleHttpRequest(const HttpRequestPtr &req,
                                std::function<void (const HttpResponsePtr &)> &&callback) override;
};
