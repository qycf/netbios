#include "Base64Codec.h"
#include <algorithm>
#include <cctype>

// 初始化Base64编码表
const std::string Base64Codec::base64Chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

std::string Base64Codec::base64Encode(const bytes& input) {
    std::string result;
    int i = 0;
    unsigned char charArray3[3];
    unsigned char charArray4[4];
    size_t inputSize = input.size();

    for (size_t n = 0; n < inputSize; ++n) {
        charArray3[i++] = input[n];
        if (i == 3) {
            charArray4[0] = (charArray3[0] & 0xfc) >> 2;
            charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xf0) >> 4);
            charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xc0) >> 6);
            charArray4[3] = charArray3[2] & 0x3f;

            for (i = 0; i < 4; ++i) {
                result += base64Chars[charArray4[i]];
            }
            i = 0;
        }
    }

    if (i > 0) {
        for (int j = i; j < 3; ++j) {
            charArray3[j] = '\0';
        }

        charArray4[0] = (charArray3[0] & 0xfc) >> 2;
        charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xf0) >> 4);
        charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xc0) >> 6);
        charArray4[3] = 64; // 填充'='

        for (int j = 0; j < i + 1; ++j) {
            result += base64Chars[charArray4[j]];
        }

        // 补充剩余的'='
        while (i++ < 3) {
            result += '=';
        }
    }

    return result;
}

bool Base64Codec::base64Decode(const std::string& input, bytes& output) {
    output.clear();
    int inLen = input.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char charArray4[4], charArray3[3];

    while (inLen-- && (input[in_] != '=') && isBase64(input[in_])) {
        charArray4[i++] = input[in_];
        in_++;
        if (i == 4) {
            for (i = 0; i < 4; ++i) {
                charArray4[i] = base64Chars.find(charArray4[i]);
            }

            charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
            charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
            charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

            for (i = 0; (i < 3) && (charArray4[i + 1] != 64); ++i) {
                output.push_back(charArray3[i]);
            }

            i = 0;
        }
    }

    // 处理最后一组
    if (i > 0) {
        for (int k = i; k < 4; ++k) {
            charArray4[k] = 0;
        }

        for (int k = 0; k < 4; ++k) {
            charArray4[k] = base64Chars.find(charArray4[k]);
        }

        charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
        charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
        charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

        for (int k = 0; (k < i - 1) && (charArray4[k + 1] != 64); ++k) {
            output.push_back(charArray3[k]);
        }
    }

    return true;
}

bool Base64Codec::isBase64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

bool Base64Codec::hexToDec(const std::string& hexStr, uint64_t& dec) {
    dec = 0;
    std::string str = hexStr;

    // 处理0x前缀
    if (str.size() >= 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        str = str.substr(2);
    }

    if (str.empty()) {
        return false;
    }

    for (char c : str) {
        dec *= 16;
        if (c >= '0' && c <= '9') {
            dec += (c - '0');
        }
        else if (c >= 'A' && c <= 'F') {
            dec += (10 + c - 'A');
        }
        else if (c >= 'a' && c <= 'f') {
            dec += (10 + c - 'a');
        }
        else {
            // 非法字符
            return false;
        }
    }
    return true;
}

std::string Base64Codec::decToHex(uint64_t dec, bool withPrefix, bool uppercase) {
    if (dec == 0) {
        return withPrefix ? "0x0" : "0";
    }

    const std::string hexDigitsLower = "0123456789abcdef";
    const std::string hexDigitsUpper = "0123456789ABCDEF";
    const std::string& hexDigits = uppercase ? hexDigitsUpper : hexDigitsLower;

    std::string hexStr;
    while (dec > 0) {
        hexStr += hexDigits[dec % 16];
        dec /= 16;
    }

    std::reverse(hexStr.begin(), hexStr.end());

    return withPrefix ? "0x" + hexStr : hexStr;
}
