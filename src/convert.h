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

bool IsZ_EI_LOWER(std::string);
bool IsZ_EI_UPPER(std::string);
bool IsZ_SU(std::string);
bool IsZ_SPACE(std::string);
bool IsZ_KA(std::string);
bool IsZ_HI(std::string);

void ToH_EI_LOWER(std::list<std::string>::iterator);
void ToH_EI_UPPER(std::list<std::string>::iterator);
void ToH_SU(std::list<std::string>::iterator);
void ToH_SPACE(std::list<std::string>::iterator);
void ToZH_HK(std::list<std::string>::iterator, uint32_t, uint32_t);

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
    std::map<const char, uint32_t> flags {
        {'V', 0x10},   // 濁点付きの文字を一文字に変換します。"K", "H" と共に使用します。
        {'r', 0x2},    // 「全角」英字を「半角」に変換します。
        {'R', 0x20},   // 「半角」英字を「全角」に変換します。
        {'n', 0x4},    // 「全角」数字を「半角」に変換します。
        {'N', 0x40},   // 「半角」数字を「全角」に変換します。
        {'a', 0x6},    // 「全角」英数字を「半角」に変換します。0x2  | 0x4
        {'A', 0x60},   // 「半角」英数字を「全角」に変換します。0x20 | 0x40
                        // （"a", "A" オプションに含まれる文字は、U+0022, U+0027, U+005C, U+007Eを除く U+0021 - U+007E の範囲です）。
        {'s', 0x26},   // 「全角」スペースを「半角」に変換します（U+3000 -> U+0020）。
        {'S', 0x260},  // 「半角」スペースを「全角」に変換します（U+0020 -> U+3000）。
        {'k', 0x680},   // 「全角カタカナ」を「半角カタカナ」に変換します。
        {'K', 0x6800},  // 「半角カタカナ」を「全角カタカナ」に変換します。

        {'h', 0x750},   // 「全角ひらがな」を「半角カタカナ」に変換します。
        {'H', 0x7500},  // 「半角カタカナ」を「全角ひらがな」に変換します。
        {'c', 0x850},  // 「全角カタカナ」を「全角ひらがな」に変換します。
        {'C', 0x8500}, // 「全角ひらがな」を「全角カタカナ」に変換します。
        {'m', 0x1000},  // 「全角」記号を「半角」に変換します。
        {'M', 0x10000}  // 「半角」記号を「全角」に変換します。
    };
    int opts = 0;
    bool Parse() {
        for (int i=0; i<(int)mOptStr.length(); i++) {
            if (flags.count(mOptStr[i]) > 0) {
                opts |= flags[mOptStr[i]];
            } else {
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

static const uint32_t HK_MAP_ZHI = 0;
static const uint32_t HK_MAP_ZKA = 1;
static const uint32_t HK_MAP_HKA = 2;
static const std::vector<std::vector<std::string>> HK_MAP = {
    { { u8"ぁ", u8"ァ", u8"ｧ" } },
    { { u8"あ", u8"ア", u8"ｱ" } },
    { { u8"ぃ", u8"ィ", u8"ｨ" } },
    { { u8"い", u8"イ", u8"ｲ" } },
    { { u8"ぅ", u8"ゥ", u8"ｩ" } },
    { { u8"う", u8"ウ", u8"ｳ" } },
    { { u8"ぇ", u8"ェ", u8"ｪ" } },
    { { u8"え", u8"エ", u8"ｴ" } },
    { { u8"ぉ", u8"ォ", u8"ｫ" } },
    { { u8"お", u8"オ", u8"ｵ" } },
    { { u8"か", u8"カ", u8"ｶ" } },
    { { u8"が", u8"ガ", u8"ｶﾞ" } },
    { { u8"き", u8"キ", u8"ｷ" } },
    { { u8"ぎ", u8"ギ", u8"ｷﾞ" } },
    { { u8"く", u8"ク", u8"ｸ" } },
    { { u8"ぐ", u8"グ", u8"ｸﾞ" } },
    { { u8"け", u8"ケ", u8"ｹ" } },
    { { u8"げ", u8"ゲ", u8"ｹﾞ" } },
    { { u8"こ", u8"コ", u8"ｺ" } },
    { { u8"ご", u8"ゴ", u8"ｺﾞ" } },
    { { u8"さ", u8"サ", u8"ｻ" } },
    { { u8"ざ", u8"ザ", u8"ｻﾞ" } },
    { { u8"し", u8"シ", u8"ｼ" } },
    { { u8"じ", u8"ジ", u8"ｼﾞ" } },
    { { u8"す", u8"ス", u8"ｽ" } },
    { { u8"ず", u8"ズ", u8"ｽﾞ" } },
    { { u8"せ", u8"セ", u8"ｾ" } },
    { { u8"ぜ", u8"ゼ", u8"ｾﾞ" } },
    { { u8"そ", u8"ソ", u8"ｿ" } },
    { { u8"ぞ", u8"ゾ", u8"ｿﾞ" } },
    { { u8"た", u8"タ", u8"ﾀ" } },
    { { u8"だ", u8"ダ", u8"ﾀﾞ" } },
    { { u8"ち", u8"チ", u8"ﾁ" } },
    { { u8"ぢ", u8"ヂ", u8"ﾁﾞ" } },
    { { u8"っ", u8"ッ", u8"ｯ" } },
    { { u8"つ", u8"ツ", u8"ﾂ" } },
    { { u8"づ", u8"ヅ", u8"ﾂﾞ" } },
    { { u8"て", u8"テ", u8"ﾃ" } },
    { { u8"で", u8"デ", u8"ﾃﾞ" } },
    { { u8"と", u8"ト", u8"ﾄ" } },
    { { u8"ど", u8"ド", u8"ﾄﾞ" } },
    { { u8"な", u8"ナ", u8"ﾅ" } },
    { { u8"に", u8"ニ", u8"ﾆ" } },
    { { u8"ぬ", u8"ヌ", u8"ﾇ" } },
    { { u8"ね", u8"ネ", u8"ﾈ" } },
    { { u8"の", u8"ノ", u8"ﾉ" } },
    { { u8"は", u8"ハ", u8"ﾊ" } },
    { { u8"ば", u8"バ", u8"ﾊﾞ" } },
    { { u8"ぱ", u8"パ", u8"ﾊﾟ" } },
    { { u8"ひ", u8"ヒ", u8"ﾋ" } },
    { { u8"び", u8"ビ", u8"ﾋﾞ" } },
    { { u8"ぴ", u8"ピ", u8"ﾋﾟ" } },
    { { u8"ふ", u8"フ", u8"ﾌ" } },
    { { u8"ぶ", u8"ブ", u8"ﾌﾞ" } },
    { { u8"ぷ", u8"プ", u8"ﾌﾟ" } },
    { { u8"へ", u8"ヘ", u8"ﾍ" } },
    { { u8"べ", u8"ベ", u8"ﾍﾞ" } },
    { { u8"ぺ", u8"ペ", u8"ﾍﾟ" } },
    { { u8"ほ", u8"ホ", u8"ﾎ" } },
    { { u8"ぼ", u8"ボ", u8"ﾎﾞ" } },
    { { u8"ぽ", u8"ポ", u8"ﾎﾟ" } },
    { { u8"ま", u8"マ", u8"ﾏ" } },
    { { u8"み", u8"ミ", u8"ﾐ" } },
    { { u8"む", u8"ム", u8"ﾑ" } },
    { { u8"め", u8"メ", u8"ﾒ" } },
    { { u8"も", u8"モ", u8"ﾓ" } },
    { { u8"ゃ", u8"ャ", u8"ｬ" } },
    { { u8"や", u8"ヤ", u8"ﾔ" } },
    { { u8"ゅ", u8"ュ", u8"ｭ" } },
    { { u8"ゆ", u8"ユ", u8"ﾕ" } },
    { { u8"ょ", u8"ョ", u8"ｮ" } },
    { { u8"よ", u8"ヨ", u8"ﾖ" } },
    { { u8"ら", u8"ラ", u8"ﾗ" } },
    { { u8"り", u8"リ", u8"ﾘ" } },
    { { u8"る", u8"ル", u8"ﾙ" } },
    { { u8"れ", u8"レ", u8"ﾚ" } },
    { { u8"ろ", u8"ロ", u8"ﾛ" } },
    { { u8"ゎ", u8"ヮ", u8"ﾜ" } },
    { { u8"わ", u8"ワ", u8"ﾜ" } },
    { { u8"ゐ", u8"ヰ", u8"ｲ" } },
    { { u8"ゑ", u8"ヱ", u8"ｴ" } },
    { { u8"を", u8"ヲ", u8"ｦ" } },
    { { u8"ん", u8"ン", u8"ﾝ" } },
    { { u8"ゔ", u8"ヴ", u8"ｳﾞ" } },
    { { u8"ー", u8"ー", u8"ｰ" } }
};

} // namespace kanaconv

#endif // KANACONV_CONVERT_H_
