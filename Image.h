#pragma once
#include <string>
#include <fstream>
#include "color.h"

struct BMHead
{
#define UC unsigned char
	UC BM[2] = { 0x42,0x4d };
	UC BMs[4];
	UC keepArea[4] = { 0x00,0x00,0x00,0x00 };
	UC sa[4] = { 0x36,0x00,0x00,0x00 };
	UC hs[4] = { 0x28,0x00,0x00,0x00 };
	UC Bw[4];
	UC Bh[4];
	UC dl[2] = { 0x01,0x00 };
	UC cb[2] = { 0x18,0x00 };
	UC nullA[24];

	BMHead(size_t w, size_t h) {
		size_t fs = w * h * 3 + 36;
		for (int i = 0; i < 4; i++) {
			BMs[i] = (UC)(fs % 256);
			fs /= 256;
		}

		for (int i = 0; i < 4; i++) {
			Bw[i] = (UC)(w % 256);
			w /= 256;
		}
		for (int i = 0; i < 4; i++) {
			Bh[i] = (UC)(h % 256);
			h /= 256;
		}
		for (int i = 0; i < 24; i++) {
			nullA[i] = 0x00;
		}
	}

	UC operator[](const int& i)const {
		return BM[i];
	}
	int size()const {
		return 54;
	}
};
class Image
{
	std::ofstream pf;
	std::string fileName;
public:
	enum format
	{
		BMP,
		PPM
	};
	size_t width, height;
	format mformat;
	~Image()
	{
		if (pf.is_open())
			pf.close();
	}
private:
	void newFile(const std::string& format, bool overwrite= true)
	{
		pf = std::ofstream((this->fileName + "." + format), std::ios::in | std::ios::binary);
		if (pf.is_open())
		{
			size_t i;
			for (i = 2; pf.is_open() && i < 1000; i++)
			{
				std::cerr << getFileName() << " already exists\n";
				pf.close();
				if (overwrite)
				{
					break;
				}
				this->fileName = fileName + "(" + std::to_string(i) + +")";
				pf.open(this->fileName + "." + format, std::ios::in);
			}	
		}
		pf.open(this->fileName + "." + format, std::ios::trunc | std::ios::binary);
	}
public:
	Image(const std::string fileName, Image::format format, const size_t& width, const size_t& height)
		: fileName(fileName), mformat(format), width(width), height(height)
	{
		switch (format)
		{
		case Image::BMP:
		{
			newFile("bmp");
			BMHead bmh(this->width, this->height);
			for (int i = 0; i < bmh.size(); i++) {
				pf << bmh[i];
			}
			break;
		}
		case Image::PPM:
		{
			newFile("ppm");
			pf << "P6\n" << this->width << " " << this->height << "\n255\n";
			break;
		}
		default:
			break;
		}
	}
	inline void open()
	{
		pf.close();
		system(("start " + getFileName()).c_str());
	}
	inline std::string getFileName()
	{
		std::string file;
		switch (mformat)
		{
		case Image::BMP:
			file = fileName + '.' + "bmp";
			break;
		case Image::PPM:
			file = fileName + '.' + "ppm";
			break;
		default:
			break;
		}
		return file;
	}
	inline void write_color(color pixel_color, int samples_per_pixel)
	{
		// Divide the color by the number of samples.
		auto scale = 1.0 / samples_per_pixel;
		pixel_color *= scale;
		char str[3];
		switch (mformat)
		{
		case Image::BMP:
		{
			char str[3] = {
			static_cast<char>(255.999 * sqrt(clamp(pixel_color.z, 0, 1))),
			static_cast<char>(255.999 * sqrt(clamp(pixel_color.y, 0, 1))),
			static_cast<char>(255.999 * sqrt(clamp(pixel_color.x, 0, 1)))
			};
			break;
		}
		case Image::PPM:
		{
			char str[3] = {
			static_cast<char>(255.999 * sqrt(clamp(pixel_color.x, 0, 1))),
			static_cast<char>(255.999 * sqrt(clamp(pixel_color.y, 0, 1))),
			static_cast<char>(255.999 * sqrt(clamp(pixel_color.z, 0, 1)))
			};
			break;
		}
		default:
			break;
		}

		pf.write(str, 3);
	}
	void write_colors(color* pixel_colors, size_t size, int samples_per_pixel)
	{
		auto scale = 1.0 / samples_per_pixel / samples_per_pixel;

		char* str = new char[size * 3];
		switch (mformat)
		{
		case Image::BMP:
			for (size_t i = 0; i < size; i++)
			{
				pixel_colors[i] *= scale;
				str[i * 3] = static_cast<char>(255.999 * sqrt(clamp(pixel_colors[i].z, 0, 1)));
				str[1 + i * 3] = static_cast<char>(255.999 * sqrt(clamp(pixel_colors[i].y, 0, 1)));
				str[2 + i * 3] = static_cast<char>(255.999 * sqrt(clamp(pixel_colors[i].x, 0, 1)));
			}
			break;
		case Image::PPM:
			for (size_t i = 0; i < size; i++)
			{
				pixel_colors[i] *= scale;
				str[i * 3] = static_cast<char>(255.999 * sqrt(clamp(pixel_colors[i].x, 0, 1)));
				str[1 + i * 3] = static_cast<char>(255.999 * sqrt(clamp(pixel_colors[i].y, 0, 1)));
				str[2 + i * 3] = static_cast<char>(255.999 * sqrt(clamp(pixel_colors[i].z, 0, 1)));
			}
			break;
		default:
			break;
		}
		pf.write(str, size * 3);
		delete[] str;
	}
	std::ofstream& getPf()
	{
		return pf;
	}
};
