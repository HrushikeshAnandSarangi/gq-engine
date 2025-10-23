#include "controllers/TestController.h"

void TestController::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    Json::Value ret;
    ret["message"]="Pong";
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}
