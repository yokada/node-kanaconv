#include "convert.h"


std::string kanaconv::hello() {
    return "あいうえお";
}

kanaconv::Utf8Chars::Utf8Chars(std::string orig_str) {
    orig_c_str_ = (uint8_t*)orig_str.c_str();
    Parse();
}

void kanaconv::Utf8Chars::Parse() {
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t k = 0;
    uint8_t* c = NULL;

    uint8_t* sp = orig_c_str_;
    while (*sp != '\0') {
        if (c == NULL) {
            c = (uint8_t*)malloc(sizeof(uint8_t) * CHAR_MAX_SIZE);
        }

        if (*sp == 0x0a || *sp == 0x0d || (*sp & 0x80) == 0) { // newline or 1-byte char
            k=0;
            c[j] = *sp;
            ++j;
        } else if ((*sp & 0xf0) == 0xf0) { // 4-byte char, first byte
            k=3;
            c[j] = *sp;
            ++j;
        } else if ((*sp & 0xf0) == 0xe0) { // 3-byte char, first byte
            k=2;
            c[j] = *sp;
            ++j;
        } else if ((*sp & 0xc0) == 0x80) { // N-byte of char, second or more byte
            --k;
            c[j] = *sp;
            ++j;
        } else { // unexpected byte
        }

        if (k==0) {
            c[j] = '\0';
            std::string str = (const char*)c;
            list_.push_back(str);

            ++i;
            j=0;
            free(c);
            c = NULL;
        }

        ++sp;
    }

    if (c != NULL) {
        free(c);
    }
}

Napi::String kanaconv::HelloWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::String returnValue = Napi::String::New(env, kanaconv::hello());
    return returnValue;
}

bool kanaconv::IsZ_EI_LOWER(std::string s) {
    if (s >= u8"ａ" && s <= u8"ｚ") {
        return true;
    }
    return false;
}
bool kanaconv::IsZ_EI_UPPER(std::string s) {
    if (s >= u8"Ａ" && s <= u8"Ｚ") {
        return true;
    }
    return false;
}
bool kanaconv::IsZ_SU(std::string s) {
    if (s >= u8"０" && s <= u8"９") {
        return true;
    }
    return false;
}
bool kanaconv::IsZ_SPACE(std::string s) {
    if (s == u8"　") {
        return true;
    }
    return false;
}
bool kanaconv::IsZ_KA(std::string s) {
    if (s >= u8"ァ" && s <= u8"ヴ") {
        return true;
    }
    return false;
}

void kanaconv::ToH_EI_LOWER(std::list<std::string>::iterator it) {
    std::string s = *it;
    *it = s[2] - 0x20;
}
void kanaconv::ToH_EI_UPPER(std::list<std::string>::iterator it) {
    std::string s = *it;
    *it = s[2]-0x60;
}
void kanaconv::ToH_SU(std::list<std::string>::iterator it) {
    std::string s = *it;
    *it = s[2]-0x60;
}
void kanaconv::ToH_SPACE(std::list<std::string>::iterator it) {
    std::string s = *it;
    *it = s[2]-0x60;
}
void kanaconv::ToH_KA(std::list<std::string>::iterator it) {
    std::string s = *it;
    if (s == u8"ァ") { *it = u8"ｧ"; }
    else if (s == u8"ア") { *it = u8"ｱ"; }
    else if (s == u8"イ") { *it = u8"ｲ"; }
    else { *it = '?'; }
}

Napi::Value kanaconv::ConvertWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 2) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string optStr = info[1].As<Napi::String>().Utf8Value();
    kanaconv::ConvertOptions opts(optStr);
    if (!opts.IsValid()) {
        Napi::TypeError::New(env, "Wrong options specified").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string buf;
    std::string str = info[0].As<Napi::String>().Utf8Value();
    Utf8Chars chars(str);
    for (auto it = chars.list_.begin(); it != chars.list_.end(); ++it) {
        if (opts.Has('r') && IsZ_EI_LOWER(*it)) {
            ToH_EI_LOWER(it);
        } else if (opts.Has('r') && IsZ_EI_UPPER(*it)) {
            ToH_EI_UPPER(it);
        } else if (opts.Has('n') && IsZ_SU(*it)) {
            ToH_SU(it);
        } else if (opts.Has('s') && IsZ_SPACE(*it)) {
            ToH_SPACE(it);
        } else if (opts.Has('k') && IsZ_KA(*it)) {
            ToH_KA(it);
        }
        buf += *it;
    }

    return Napi::String::New(env, buf);
}

Napi::Object kanaconv::Init(Napi::Env env, Napi::Object exports) {
    exports.Set("hello", Napi::Function::New(env, kanaconv::HelloWrapped));
    exports.Set("convert", Napi::Function::New(env, kanaconv::ConvertWrapped));

    return exports;
}
