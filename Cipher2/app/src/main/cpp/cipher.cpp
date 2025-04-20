// Write C++ code here.
//
// Do not forget to dynamically load the C++ library into your application.
//
// For instance,
//
// In MainActivity.java:
//    static {
//       System.loadLibrary("cipher");
//    }
//
// Or, in MainActivity.kt:
//    companion object {
//      init {
//         System.loadLibrary("cipher")
//      }
//    }

#include <jni.h>
#include <string>
#include <functional>
#include <string>
#include <regex>
#include <sstream>
#include <cctype>
#include <algorithm>

extern "C" JNIEXPORT jstring JNICALL
Java_com_moonkey_cipher_MainActivity_cipherNative(
        JNIEnv* env,
        jobject /* this */,
        jstring text1,
        jstring text2) {
    const char* c_text1 = env->GetStringUTFChars(text1, nullptr);
    const char* c_text2 = env->GetStringUTFChars(text2, nullptr);

    std::string salt = std::string(c_text1);
    std::string input = std::string(c_text2);

    const int subList[5] = {1, 2, 3, 5, 7};
    const std::regex regexNotChar("[^A-Za-z0-9]");
    std::string filteredSalt = std::regex_replace(salt, regexNotChar, "");
    if (filteredSalt.empty()) return env->NewStringUTF("invalid salt");
    if (input.length() < 6) return env->NewStringUTF("invalid input");

    std::string rawSign = input.substr(0, 5);
    const std::string special = "()`!@#$%^&*_-+=|{}[]:;'<>,.?";
    std::string sign;
    for (const char &c : rawSign) if (isalnum(c) || special.find(c) != std::string::npos) sign.push_back(c);
    if (sign.length() < 5) return env->NewStringUTF("invalid sign");

    std::string filteredInput = std::regex_replace(input.substr(5), regexNotChar, "");
    if (filteredInput.empty()) return env->NewStringUTF("invalid input");

    std::string output;
    for (int i : subList) {
        if ((size_t)(i-1) >= filteredInput.length()) break;
        output += filteredInput[i - 1];
    }

    if (filteredInput.length() == 1) {
        output += std::string(3, filteredInput[0]);
    } else if (filteredInput.length() == 2) {
        output += filteredInput + filteredInput.back();
    } else {
        output += filteredInput.substr(filteredInput.length() - 3);
    }

    for (int i = 0; i < output.length(); ++i) if (output[i] >= 'A' && output[i] <= 'Z') output[i] += 32;
    if (!output.empty()) if (output[0] >= 'a' && output[0] <= 'z') output[0] -= 32;

    int linePos = 0;
    if (isdigit(output[0])) {
        linePos = output[0] - '0';
    } else {
        int upper = std::toupper(output[0]) - 'A';
        linePos = (upper / 10) + (upper % 10);
    }

    const int primeList[12] = {37, 31, 29, 23, 19, 17, 13, 11, 7, 5, 3, 2};
    const std::string base = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    auto generateCipher = [&]() {
        std::string combined = (output + filteredSalt).substr(0, 12);
        int sum = 0;
        for (size_t i = 0; i < combined.length(); ++i)
            sum += static_cast<unsigned char>(combined[i]) * primeList[i];

        std::ostringstream oss;
        oss << base[sum % 62] << (sum % 10) << base[(sum / 62) % 62];
        return oss.str();
    };

    std::string cipher = generateCipher();
    std::string code = sign.substr(0, 2) + output + sign.substr(3, 1) + cipher + sign.substr(4, 1);
    size_t insertPos = (linePos < 0) ? 0 : std::min((size_t)linePos, code.length());
    code.insert(insertPos, sign.substr(2, 1));
    output = code;

    env->ReleaseStringUTFChars(text1, c_text1);
    env->ReleaseStringUTFChars(text2, c_text2);
    return env->NewStringUTF(output.c_str());
}