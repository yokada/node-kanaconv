#ifndef KANACONV_CONVERT_H_
#define KANACONV_CONVERT_H_

#include <napi.h>
#include <iostream>
#include <map>
#include <string>
#include <list>

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif


namespace kanaconv {

std::string hello();
Napi::String HelloWrapped(const Napi::CallbackInfo& info);
Napi::Value ConvertWrapped(const Napi::CallbackInfo& info);

static const uint32_t CHARS_DEFAULT_SIZE = 256;
static const uint32_t CHAR_MAX_SIZE = 5;
static const uint8_t Z_SU_START[] = {0xef, 0xbc, 0x90}; // "０"
static const uint8_t Z_SU_END[]  = {0xef, 0xbc, 0x99}; // "９";
static const uint8_t Z_SPACE[] = {0xe3, 0x80, 0x80}; // 全角スペース
static const uint8_t Z_KA_START[] = {0xe3, 0x82, 0xa1}; // "ァ"（拗音）
static const uint8_t Z_KA_END[] = {0xe3, 0x83, 0xb4}; // ヴ
static const uint8_t Z_HI_START[] = {0xe3, 0x81, 0x81}; // ぁ（拗音）
static const uint8_t Z_HI_END[] = {0xe8, 0x82, 0x96}; // ゖ

bool IsZ_EI_LOWER(std::string);
bool IsZ_EI_UPPER(std::string);
bool IsZ_SU(std::string);
bool IsZ_SPACE(std::string);
bool IsZ_KA(std::string);

void ToH_EI_LOWER(std::list<std::string>::iterator);
void ToH_EI_UPPER(std::list<std::string>::iterator);
void ToH_SU(std::list<std::string>::iterator);
void ToH_SPACE(std::list<std::string>::iterator);
void ToH_KA(std::list<std::string>::iterator);

Napi::Object Init(Napi::Env env, Napi::Object exports);

class ConvertOptions {
public:
    ConvertOptions(std::string optStr) {
        mOptStr = optStr;
    }
    bool IsValid() {
        return Parse();
    }
    bool Has(char o) {
        if (flags.count(o) <= 0) {
            return false;
        }
        return (bool)(opts & flags[o]);
    }
private:
    std::string mOptStr;
    std::map<const char, int> flags {
        {'V', 0x10},   // 濁点付きの文字を一文字に変換します。"K", "H" と共に使用します。
        {'r', 0x2},    // 「全角」英字を「半角」に変換します。
        {'R', 0x20},   // 「半角」英字を「全角」に変換します。
        {'n', 0x4},    // 「全角」数字を「半角」に変換します。
        {'N', 0x40},   // 「半角」数字を「全角」に変換します。
        {'a', 0x6},    // 「全角」英数字を「半角」に変換します。0x2  | 0x4
        {'A', 0x60},   // 「半角」英数字を「全角」に変換します。0x20 | 0x40
                        // （"a", "A" オプションに含まれる文字は、U+0022, U+0027, U+005C, U+007Eを除く U+0021 - U+007E の範囲です）。
        {'s', 0x20},   // 「全角」スペースを「半角」に変換します（U+3000 -> U+0020）。
        {'S', 0x200},  // 「半角」スペースを「全角」に変換します（U+0020 -> U+3000）。
        {'k', 0x80},   // 「全角カタカナ」を「半角カタカナ」に変換します。
        {'K', 0x800},  // 「半角カタカナ」を「全角カタカナ」に変換します。

        {'h', 0x75},   // 「全角ひらがな」を「半角カタカナ」に変換します。
        {'H', 0x750},  // 「半角カタカナ」を「全角ひらがな」に変換します。
        {'c', 0x85},  // 「全角カタカナ」を「全角ひらがな」に変換します。
        {'C', 0x850}, // 「全角ひらがな」を「全角カタカナ」に変換します。
        {'m', 0x950},  // 「全角」記号を「半角」に変換します。
        {'M', 0x9500}  // 「半角」記号を「全角」に変換します。
    };
    int opts = 0;
    bool Parse() {
        for (int i=0; i<(int)mOptStr.length(); i++) {
            if (flags.count(mOptStr[i]) > 0) {
                opts |= flags[mOptStr[i]];
            } else {
                // todo
                //throw "有効なオプションではありません:"; // -fno-exceptions なので throw 禁止
                return false;
            }
        }

        DEBUG_PRINT("opts: %i\n", opts);
        return true;
    }
};

class Utf8Chars {
public:
    Utf8Chars(std::string);
    std::list<std::string> list_;
    std::string ConvertKana(std::string, ConvertOptions);

private:
    uint8_t* orig_c_str_;
    void Parse();
};

} // namespace kanaconv

#endif // KANACONV_CONVERT_H_
