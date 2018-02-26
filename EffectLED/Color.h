// Color.h

#ifndef _COLOR_h
#define _COLOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

static const uint32_t int_to_uint8(int value) {
	return value < 0 ? 0 : (value > 255 ? 255 : (uint32_t)value);
}

class Color {
private:
	uint32_t _rawValue;
	uint8_t red, green, blue, white;
public:
	uint32_t rawValue() {
		return _rawValue;
	}
	static const uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
		return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
	}
	static const uint32_t rgbw(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
		return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
	}
	static const uint32_t rgb_clip(int r, int g, int b) {
		return (int_to_uint8(r) << 16) | (int_to_uint8(g) << 8) | int_to_uint8(b);
	}
	static const uint32_t rgbw_clip(int r, int g, int b, int w) {
		return (int_to_uint8(w) << 24) | (int_to_uint8(r) << 16) | (int_to_uint8(g) << 8) | int_to_uint8(b);
	}
	Color() {
		red = 0; green = 0; blue = 0; white = 0;
		_rawValue = 0;
	}
	Color(int r, int g, int b) {
		red = int_to_uint8(r); green = int_to_uint8(g); blue = int_to_uint8(b); white = 0;
		_rawValue = rgb_clip(r, g, b);
	}
	Color(int r, int g, int b, int w) {
		red = int_to_uint8(r); green = int_to_uint8(g); blue = int_to_uint8(b); white = int_to_uint8(w);
		_rawValue = rgbw_clip(r, g, b, w);
	}
	Color(uint32_t raw) {
		_rawValue = raw;
		red = raw;
		green = raw << 8;
		blue = raw << 16;
		white = raw << 24;
	}
	static Color RED ;
	static Color GREEN;
	static Color BLUE;
	static Color YELLOW;
	static Color WHITE;
	static Color BLACK;

	Color operator+ (Color other) {
		return Color(red + other.red, green + other.green, blue + other.blue, white + other.white);
	}
	Color operator- (Color other) {
		return Color(red - other.red, green - other.green, blue - other.blue, white - other.white);
	}
	Color operator* (float factor) {
		return Color((int)(red * factor), (int)(green * factor), (int)(blue * factor), (int)(white * factor));
	}
};


#endif

