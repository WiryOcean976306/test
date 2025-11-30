#include <string>
#include <regex>
#include <stdexcept>
#include <ctime>
#include <chrono>

using namespace std;


struct CardInfo {
    string cardNumber;
    string expiryDate;
    string cvv;

    bool empty() const {
        return cardNumber.empty() && expiryDate.empty() && cvv.empty();
    }
};


class AccountRegistration {
private:
    string email;
    string password;
    string address;
    CardInfo creditCard;

    bool validateEmail(const string& email) const {
        regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
        return regex_match(email, emailPattern);
    }

    bool validatePassword(const string& password) const {
        return password.length() >= 8;
    }

    bool isCardExpired(const string& expiryDate) const {
        // expiryDate format: MM/YY
        if (expiryDate.length() != 5 || expiryDate[2] != '/') {
            return true; // Invalid format considered expired
        }
        try {
            int month = stoi(expiryDate.substr(0, 2));
            int year = stoi(expiryDate.substr(3, 2));
            
            // Get current date
            auto now = chrono::system_clock::now();
            time_t t = chrono::system_clock::to_time_t(now);
            struct tm* timeinfo = localtime(&t);
            
            int currentYear = timeinfo->tm_year % 100; // YY format
            int currentMonth = timeinfo->tm_mon + 1;   // tm_mon is 0-11
            
            // Card expires at end of month, so compare: if current > expiry, it's expired
            if (year > currentYear) return false;  // Future year
            if (year < currentYear) return true;   // Past year
            return month < currentMonth;           // Same year, compare months
        } catch (...) {
            return true; // Invalid date format
        }
    }

    bool validateCreditCard(const CardInfo& card) const {
        // Accept empty card info (all fields empty)
        if (card.empty()) return true;
        regex numberPattern(R"(^\d{16}$)");
        regex expiryPattern(R"(^\d{2}/\d{2}$)");
        regex cvvPattern(R"(^\d{3}$)");
        return regex_match(card.cardNumber, numberPattern) &&
               regex_match(card.expiryDate, expiryPattern) &&
               regex_match(card.cvv, cvvPattern) &&
               !isCardExpired(card.expiryDate);
    }

public:
    void registerAccount(const string& email, const string& password,
                        const string& address = "", const CardInfo& creditCard = CardInfo{}) {
        if (!validateEmail(email)) {
            throw invalid_argument("Invalid email format");
        }
        if (!validatePassword(password)) {
            throw invalid_argument("Password must be at least 8 characters");
        }
        if (!validateCreditCard(creditCard)) {
            throw invalid_argument("Invalid credit card format");
        }

        this->email = email;
        this->password = password;
        this->address = address;
        this->creditCard = creditCard;
    }

    void setAddress(const string& address) {
        this->address = address;
    }

    void setCreditCard(const CardInfo& creditCard) {
        if (!validateCreditCard(creditCard)) {
            throw invalid_argument("Invalid credit card format");
        }
        this->creditCard = creditCard;
    }

    string getEmail() const { return email; }
    string getAddress() const { return address; }
    bool hasCreditCard() const { return !creditCard.empty(); }
};