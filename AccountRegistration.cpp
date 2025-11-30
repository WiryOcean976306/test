#include <string>
#include <regex>
#include <stdexcept>

using namespace std;


struct CardInfo {
    string cardNumber;
    string expiryDate;
    string cvv;
};


class AccountRegistration {
private:
    string email;
    string password;
    string address;
    string creditCard;

    bool validateEmail(const string& email) const {
        regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
        return regex_match(email, emailPattern);
    }

    bool validatePassword(const string& password) const {
        return password.length() >= 8;
    }

    bool validateCreditCard(const CardInfo& card) const {
        return card.empty() || 
               (regex_match(card, regex(R"(^\d{16}|^$)")) &&
                regex_match(card, regex(R"(^\d{2}/\d{2}|^$)")) &&
                regex_match(card, regex(R"(^\d{3}|^$)")));
    }

public:
    void registerAccount(const string& email, const string& password,
                        const string& address = "", const string& creditCard = "") {
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

    void setCreditCard(const string& creditCard) {
        if (!validateCreditCard(creditCard)) {
            throw invalid_argument("Invalid credit card format");
        }
        this->creditCard = creditCard;
    }

    string getEmail() const { return email; }
    string getAddress() const { return address; }
    bool hasCreditCard() const { return !creditCard.empty(); }
};