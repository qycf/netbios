#pragma once
// map序列化工具类
class MapSerializer {
public:
    // 序列化map为bytes
    static bool serialize(const std::map<std::string, std::pair<bool, int>>& mapData, bytes& outData) {
        // 先写入map大小
        size_t mapSize = mapData.size();
        const char* sizePtr = reinterpret_cast<const char*>(&mapSize);
        outData.insert(outData.end(), sizePtr, sizePtr + sizeof(size_t));

        // 写入每个键值对
        for (const auto& entry : mapData) {
            // 写入字符串长度
            size_t strLen = entry.first.length();
            const char* lenPtr = reinterpret_cast<const char*>(&strLen);
            outData.insert(outData.end(), lenPtr, lenPtr + sizeof(size_t));

            // 写入字符串内容
            outData.insert(outData.end(), entry.first.begin(), entry.first.end());

            // 写入bool值
            bool flag = entry.second.first;
            const char* flagPtr = reinterpret_cast<const char*>(&flag);
            outData.insert(outData.end(), flagPtr, flagPtr + sizeof(bool));

            // 写入int值
            int value = entry.second.second;
            const char* valuePtr = reinterpret_cast<const char*>(&value);
            outData.insert(outData.end(), valuePtr, valuePtr + sizeof(int));
        }

        return true;
    }

    // 从bytes反序列化map
    static bool deserialize(const bytes& inData, std::map<std::string, std::pair<bool, int>>& mapData) {
        mapData.clear();
        size_t pos = 0;
        const size_t dataSize = inData.size();

        // 读取map大小
        if (pos + sizeof(size_t) > dataSize) return false;
        size_t mapSize;
        std::memcpy(&mapSize, &inData[pos], sizeof(size_t));
        pos += sizeof(size_t);

        // 读取每个键值对
        for (size_t i = 0; i < mapSize; ++i) {
            // 读取字符串长度
            if (pos + sizeof(size_t) > dataSize) return false;
            size_t strLen;
            std::memcpy(&strLen, &inData[pos], sizeof(size_t));
            pos += sizeof(size_t);

            // 读取字符串内容
            if (pos + strLen > dataSize) return false;
            std::string key(reinterpret_cast<const char*>(&inData[pos]), strLen);
            pos += strLen;

            // 读取bool值
            if (pos + sizeof(bool) > dataSize) return false;
            bool flag;
            std::memcpy(&flag, &inData[pos], sizeof(bool));
            pos += sizeof(bool);

            // 读取int值
            if (pos + sizeof(int) > dataSize) return false;
            int value;
            std::memcpy(&value, &inData[pos], sizeof(int));
            pos += sizeof(int);

            // 添加到map
            mapData[key] = std::make_pair(flag, value);
        }

        return true;
    }

    //// 将map序列化并以Base64格式保存到文件
    //static bool saveToFile(const std::map<std::string, std::pair<bool, int>>& mapData, const std::string& filename) {
    //    bytes serializedData;
    //    if (!serialize(mapData, serializedData)) {
    //        return false;
    //    }

    //    // 转换为Base64
    //    std::string base64Str = Base64Codec::base64Encode(serializedData);

    //    // 保存到文件
    //    std::ofstream file(filename, std::ios::out | std::ios::binary);
    //    if (!file.is_open()) {
    //        return false;
    //    }

    //    file.write(base64Str.c_str(), base64Str.length());
    //    file.close();

    //    return true;
    //}

    //// 从Base64格式的文件加载并反序列化map
    //static bool loadFromFile(const std::string& filename, std::map<std::string, std::pair<bool, int>>& mapData) {
    //    // 从文件读取Base64字符串
    //    std::ifstream file(filename, std::ios::in | std::ios::binary);
    //    if (!file.is_open()) {
    //        return false;
    //    }

    //    std::string base64Str((std::istreambuf_iterator<char>(file)),
    //        std::istreambuf_iterator<char>());
    //    file.close();

    //    // 解码Base64
    //    bytes serializedData;
    //    if (!Base64Codec::base64Decode(base64Str, serializedData)) {
    //        return false;
    //    }

    //    // 反序列化
    //    return deserialize(serializedData, mapData);
    //}
};