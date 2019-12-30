#ifndef FULLCONTACT_CPP_FULLCONTACT_H
#define FULLCONTACT_CPP_FULLCONTACT_H

#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <vector>
#include <sstream>

using json = nlohmann::json;

namespace fullcontact{
    class FullContact{
    public:
        json api_get(std::string API_KEY, std::string endpoint, json args);
        int get_last_status_code();
        int get_rate_limit_remaining();

    private:
        static size_t write_response(void *ptr, size_t size, size_t count, void *stream);
        static size_t header_callback(void *ptr, size_t size, size_t nitems, void *userdata);

        std::string URL = "https://api.fullcontact.com/";

        long http_code;
        std::stringstream fc_header;
        std::string options;
        std::string response_;
        json response_js;

        json endpoints = {
                {"person", "person.enrich"},
                {"company", "company.enrich"},
                {"email_verification", "verification/email"},
                {"disposable", "email/disposable.json"},
                {"name_normalizer", "name/normalizer.json"},
                {"name_deducer", "name/deducer.json"},
                {"name_similarity", "name/similarity.json"},
                {"name_stats", "name/stats.json"},
                {"address_locationNormalizer", "address/locationNormalizer.json"},
                {"address_locationEnrichment", "address/locationEnrichment.json"},
                {"account_stats", "stats.json"}
        }; // Do all of them work?
    };
}

#endif //FULLCONTACT_CPP_FULLCONTACT_H
