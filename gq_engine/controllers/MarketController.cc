
#include "MarketController.h"
#include "models/Orders.h"
#include <drogon/orm/Mapper.h>

void MarketController::asyncHandleHttpRequest(const HttpRequestPtr &req,
                                              std::function<void (const HttpResponsePtr &)> &&callback) {
    if (req->getMethod() == Post) {
        // Original 'create' logic
        auto json = req->getJsonObject();
        if (!json) {
            Json::Value err;
            err["error"] = "Invalid JSON format";
            auto resp = HttpResponse::newHttpJsonResponse(err);
            resp->setStatusCode(k400BadRequest);
            callback(std::move(resp));
            return;
        }

        drogon_model::gq_engine::Orders newOrder(*json);
        newOrder.setStatus("OPEN");
        auto dbClient = drogon::app().getDbClient();
        drogon::orm::Mapper<drogon_model::gq_engine::Orders> mapper(dbClient);
        
        mapper.insert(newOrder,
            [callback](const drogon_model::gq_engine::Orders &order) {
                Json::Value ret;
                ret["message"] = "Order accepted";
                ret["order_id"] = *order.getId();
                auto resp = HttpResponse::newHttpJsonResponse(ret);
                callback(std::move(resp));
            },
            [callback](const drogon::orm::DrogonDbException &e) {
                Json::Value err;
                err["error"] = "Failed to save order";
                LOG_ERROR << e.base().what();
                auto resp = HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(k500InternalServerError);
                callback(std::move(resp));
            });
        return;
    } else if (req->getMethod() == Get) {
        // Original 'showAll' logic
        auto dbClient = drogon::app().getDbClient();
        drogon::orm::Mapper<drogon_model::gq_engine::Orders> mapper(dbClient);

        mapper.findAll(
            [callback](const std::vector<drogon_model::gq_engine::Orders> &orders) {
                Json::Value jsonArray(Json::arrayValue);
                for (const auto &order : orders) {
                    jsonArray.append(order.toJson());
                }
                auto resp = HttpResponse::newHttpJsonResponse(jsonArray);
                callback(std::move(resp));
            },
            [callback](const drogon::orm::DrogonDbException &e) {
                Json::Value err;
                err["error"] = "Failed to fetch orders";
                LOG_ERROR << e.base().what();
                auto resp = HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(k500InternalServerError);
                callback(std::move(resp));
            });
        return;
    } else {
        // Handle unsupported methods
        Json::Value err;
        err["error"] = "Method not allowed";
        auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k405MethodNotAllowed);
        callback(std::move(resp));
    }
}
