#include <iostream>
#include "fullcontact.h"
#include <nlohmann/json.hpp>
int main() {
    fullcontact::FullContact fc;

    json params = {
            {"email", "snriedel85@gmail.com"}
    }; // params as json find more at https://docs.fullcontact.com/


    json fc_response = fc.api_get("xgtbJvVos2xcFMX1JvXaQvx0ZaExhSCT", "person", params);
    std::cout << fc_response.dump() << std::endl; // response as json
    std::cout << fc.get_rate_limit_remaining() << std::endl; // last registered rate limit
    std::cout << fc.get_last_status_code() << std::endl; // Last registered status code
}
