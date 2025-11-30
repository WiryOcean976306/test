#include <iostream>
#include <algorithm>
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

std::string FixString(const std::string str) 
{
    std::string result = str;
    result.erase(std::remove(result.begin(), result.end(), '"'), result.end());
    return result;
}

int main() {
    // Test parsing a JSON string
    std::string json_string = R"(
    {
        "name": "John",
        "age": 30,
        "city": "New York",
        "hobbies": ["reading", "gaming", "coding"]
    }
    )";

    try {
        // Parse the JSON string
        json parsed = json::parse(json_string);

        // Access and print parsed values
        std::cout << "Name: " << FixString(parsed["name"]) << std::endl;
        std::cout << "Age: " << parsed["age"] << std::endl;
        std::cout << "City: " << FixString(parsed["city"]) << std::endl;

        std::cout << "Hobbies: ";
        for (const auto& hobby : parsed["hobbies"]) {
            std::cout << FixString(hobby) << " ";
        }
        std::cout << std::endl;

        std::cout << "\nFull parsed JSON:\n" << parsed.dump(2) << std::endl;

    } catch (const json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}