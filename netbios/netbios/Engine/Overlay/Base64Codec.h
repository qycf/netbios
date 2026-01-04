#ifndef BASE64CODEC_H
#define BASE64CODEC_H

#include <vector>
#include <string>
#include <cstdint>

// 定义字节向量类型
typedef std::vector<unsigned char> bytes;

class Base64Codec {
public:
    /**
     * Base64编码
     * @param input 输入字节向量
     * @return 编码后的字符串
     */
    static std::string base64Encode(const bytes& input);

    /**
     * Base64解码
     * @param input 输入Base64字符串
     * @param output 输出解码后的字节向量
     * @return 解码成功返回true，失败返回false
     */
    static bool base64Decode(const std::string& input, bytes& output);

    /**
     * 十六进制字符串转十进制整数
     * @param hexStr 十六进制字符串(支持0x前缀，大小写字母)
     * @param dec 输出十进制整数
     * @return 转换成功返回true，失败返回false
     */
    static bool hexToDec(const std::string& hexStr, uint64_t& dec);

    /**
     * 十进制整数转十六进制字符串
     * @param dec 十进制整数
     * @param withPrefix 是否带0x前缀
     * @param uppercase 是否使用大写字母
     * @return 转换后的十六进制字符串
     */
    static std::string decToHex(uint64_t dec, bool withPrefix = false, bool uppercase = true);

private:
    // Base64编码表
    static const std::string base64Chars;

    // 检查字符是否为Base64有效字符
    static bool isBase64(unsigned char c);
};

#endif // BASE64CODEC_H
