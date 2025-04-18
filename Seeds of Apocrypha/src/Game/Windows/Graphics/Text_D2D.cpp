#include <sstream>
#include "Text_D2D.h"

namespace Engine {

void Text_D2D::SetStr(string new_str) {

	//If the string is too long, insert some newline characters
	istringstream parser(new_str);
	string word = "", test_line = "", line = "", full_str = "";
	std::wstring wtest;
	int len = 0;

	ComPtr<IDWriteTextLayout> layout;
	IDWriteFactory* dw_factory = font->GetFactory();
	IDWriteTextFormat* format = font->GetFormat(info.char_size);
	HRESULT hr = S_OK;
    DWRITE_TEXT_METRICS metrics;

	while (parser >> word) {
		test_line = line + word + " ";

		// Convert test_line to wide string
        len = MultiByteToWideChar(CP_UTF8, 0, test_line.c_str(), -1, nullptr, 0);
		if (!len) continue;
        wtest.resize(len-1);
        MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, &wtest[0], len);

        //Measure width with DirectWrite
        hr = dw_factory->CreateTextLayout(
            wtest.c_str(),
            (UINT32)wtest.length(),
            format,
            FLT_MAX, FLT_MAX,
            &layout
        );
        if (FAILED(hr)) continue;

        layout->GetMetrics(&metrics);

		if (metrics.width > info.max_width) {
			//Remove the trailing space
			line.pop_back();
			//Add the current line plus a newline to the final string
			full_str += line + "\n";
			//Line now starts fresh
			line = "";
		}
	}
	if (!line.empty()) {
		//Remove the trailing space
		line.pop_back();
		//Append the last line
		full_str += line;
	}


	info.str = full_str;

	//Set str_size
}
}