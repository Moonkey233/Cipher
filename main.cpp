#include <iostream>
#include <functional>
int main() {
    std::string salt, sign, input, output;
    const int subList[5] = {1, 2, 3, 5, 7};
    const int primeList[12] = {37, 31, 29, 23, 19, 17, 13, 11, 7, 5, 3, 2};
    const std::string special = "()`!@#$%^&*_-+=|{}[]:;'<>,.?";
    const std::string base = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const std::function<void(std::string &)> get_valid_salt = [&get_valid_salt](std::string &saltBuf) {
        saltBuf.clear();
        std::cin >> saltBuf;
        std::string rawSalt;
        for (const char &c : saltBuf) if (isalnum(c)) rawSalt.push_back(c);
        if (rawSalt.empty()) get_valid_salt(saltBuf);
        saltBuf = rawSalt;
    };
    const std::function<void(std::string &, std::string &)> get_valid_input = [&get_valid_input, &special](std::string &signBuf, std::string &inputBuf) {
        signBuf.clear();
        inputBuf.clear();
        std::cin >> inputBuf;
        std::string rawSign = inputBuf.substr(0, std::min(static_cast<size_t>(5), inputBuf.length()));
        for (const char &c : rawSign) if (isalnum(c) || special.find(c) != std::string::npos) signBuf.push_back(c);
        const std::string rawInput = inputBuf.substr(std::min(static_cast<size_t>(5), inputBuf.length()-1));
        inputBuf.clear();
        for (const char &c : rawInput) if (isalnum(c)) inputBuf.push_back(c);
        if (signBuf.length() < 5 || inputBuf.empty()) get_valid_input(signBuf, inputBuf);
    };
    const std::function<std::string()> generate_cipher = [&output, &salt, &primeList, &base]() -> std::string {
        const std::string cipher = (output + salt).substr(0, std::min(static_cast<size_t>(12), (output + salt).length()));
        int sum = 0;
        for (int i = 0; i < cipher.length(); ++i) sum += cipher[i] * primeList[i];
        std::string code;
        code.push_back(base[sum % 62]);
        code.push_back(static_cast<char>('0' + sum % 10));
        code.push_back(base[(sum/62)%62]);
        return code;
    };
    get_valid_salt(salt);
    get_valid_input(sign, input);
    for (const int &i : subList) {
        if (input.length() < i) break;
        output.push_back(input[i-1]);
    }
    output += input.size() == 1 ? input + input + input : input.size() == 2 ? input + input.back() : input.substr(input.size() - 3);
    for (int i = 0; i < output.length(); ++i) if (output[i] >= 'A' && output[i] <= 'Z') output[i] += 32;
    if (output[0] >= 'a' && output[0] <= 'z') output[0] -= 32;
    const int linePos = output[0] >= '0' && output[0] <= '9' ? output[0] - '0' : (output[0] - 'A') / 10 + (output[0] - 'A') % 10;
    output = sign.substr(0, 2) + output + sign[3] + generate_cipher() + sign[4];
    output = output.insert(linePos, 1, sign[2]);
    std::cout << output << std::endl;
    system("pause");
    return 0;
}