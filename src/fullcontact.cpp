#include <iostream>
#include "fullcontact.h"
#include <regex>
#include <sstream>


using namespace fullcontact;




int rate_limit_remaining = 0;

size_t FullContact::header_callback(void *ptr, size_t size, size_t nitems, void *userdata) {
    char *x = static_cast<char *>(calloc(size + 1, nitems));
    assert(x);
    memcpy(x, ptr, size * nitems);
    std::string s (x);

    std::regex r("X-Rate-Limit-Remaining: ([0-9]*)");
    std::vector<int> results;
    for(std::sregex_iterator i = std::sregex_iterator(s.begin(), s.end(), r);
        i != std::sregex_iterator();
        ++i){
        std::smatch m = *i;
        if (!i->empty()){
            rate_limit_remaining = std::stoi( m[1].str().c_str());
        }
    }


    return size * nitems;

}

size_t FullContact::write_response(void *ptr, size_t size, size_t count, void *stream) {
    ((std::string*)stream)->append((char*)ptr, 0, size*count);
    return size*count;
}

json FullContact::api_get(std::string API_KEY, std::string endpoint, json args) {

    if (endpoints.find(endpoint) == endpoints.end()){
        throw std::logic_error("Unknown Endpoint!");
    }

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl){
        if (endpoints.find(endpoint)->get<std::string>() == "person.enrich" | endpoints.find(endpoint)->get<std::string>() == "company.enrich"){
            URL = URL + "v3/";
            options += "{";
            for (json::iterator it = args.begin(); it != args.end(); ++it){
                options += "\"" + it.key() + "\"" + ":" + it.value().dump();
            }
            options += "}";
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS , options.c_str());
            URL = URL + endpoints.find(endpoint)->get<std::string>();
        }
        else{
            URL = URL + "v2/";
            for (json::iterator it = args.begin(); it != args.end(); ++it){
                options += it.key() + "=" + it.value().dump() + "&";
            }
            options = std::regex_replace(options, std::regex(R"(\&$)"), "");
            URL = URL + endpoints.find(endpoint)->get<std::string>() + "?" + options;

        }

        curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());

        struct curl_slist *headers = NULL;
        fc_header << "Authorization: Bearer " << API_KEY;

        headers = curl_slist_append(headers, fc_header.str().c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_response);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_);
        std::string s;
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, &header_callback);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (CURLE_OK == res){
            curl_easy_cleanup(curl);
            return json::parse(response_);
        }
        else{
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
        return response_js;
    }
}

int FullContact::get_last_status_code() {
    return http_code;
}

int FullContact::get_rate_limit_remaining() {
    return rate_limit_remaining;
}