#ifndef ENCODING_H
#define ENCODING_H

#include <string>
#include <iconv.h>

std::string cp1251ToUtf8(const std::string& text) {
    iconv_t cd = iconv_open("UTF-8", "CP1251");

    if (cd == (iconv_t) - 1) {
        return text;
    }

    size_t inputSize = text.size();
    size_t outputSize = inputSize * 4 + 1;

    std::string result(outputSize, '\0');

    char* input = const_cast<char*>(text.data());
    char* output = result.data();
    size_t outputLeft = outputSize;

    iconv(cd, &input, &inputSize, &output, &outputLeft);
    iconv_close(cd);

    result.resize(outputSize - outputLeft);

    return result;
}

#endif