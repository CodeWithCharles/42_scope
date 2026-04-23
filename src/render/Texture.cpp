#include <glad/glad.h>

#include "render/Texture.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
	std::string	readNextPpmToken(std::ifstream& file)
	{
		std::string token;

		while (file >> token)
		{
			if (!token.empty() && token[0] == '#')
			{
				std::string ignoredLine;
				std::getline(file, ignoredLine);
				continue;
			}
			return token;
		}
		throw std::runtime_error("Unexpected end of PPM header");
	}
}

namespace Scop
{
	Texture::Texture(const std::string& path)
		:	m_id(0),
			m_width(0),
			m_height(0)
	{
		loadFromPpm(path);
	}

	Texture::~Texture()
	{
		if (m_id != 0)
			glDeleteTextures(1, &m_id);
	}

	void	Texture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	int		Texture::getWidth() const
	{
		return m_width;
	}

	int		Texture::getHeight() const
	{
		return m_height;
	}

	void	Texture::loadFromPpm(const std::string& path)
	{
		std::ifstream	file(path.c_str(), std::ios::binary);
		if (!file.is_open())
			throw std::runtime_error("Failed to open PPM file: " + path);

		std::string		magicNumber = readNextPpmToken(file);
		if (magicNumber != "P6" && magicNumber != "P3")
			throw std::runtime_error("Unsupported PPM format (expected P3|P6): " + path);

		m_width = std::stoi(readNextPpmToken(file));
		m_height = std::stoi(readNextPpmToken(file));

		if (m_width <= 0 || m_height <= 0)
			throw std::runtime_error("Invalid PPM dimensions: " + path);

		int	maxValue = std::stoi(readNextPpmToken(file));
		if (maxValue != 255)
			throw std::runtime_error("Unsupported PPM max value (expected 255): " + path);

		char	separator;
		file.get(separator);
		if (!file)
			throw std::runtime_error("Failed to read PPM pixel data separator: " + path);

		if (separator == '\r')
		{
			if (file.peek() == '\n')
				file.get();
		}
		else if (separator != '\n' && separator != ' ' && separator != '\t')
			throw std::runtime_error("Invalid PPM header separator: " + path);

		std::size_t	pixelDataSize = static_cast<std::size_t>(m_width)
			* static_cast<std::size_t>(m_height) * 3;

		std::vector<unsigned char>	pixels(pixelDataSize);

		if (magicNumber == "P6")
		{
			file.read(
				reinterpret_cast<char*>(pixels.data()),
				static_cast<std::streamsize>(pixelDataSize));

			if (file.gcount() != static_cast<std::streamsize>(pixelDataSize))
				throw std::runtime_error("Incomplete PPM pixel data: " + path);
		}
		else
		{
			for (std::size_t i = 0; i < pixelDataSize; ++i)
			{
				int componentValue = std::stoi(readNextPpmToken(file));

				if (componentValue < 0 || componentValue > 255)
					throw std::runtime_error("Invalid PPM pixel component: " + path);

				pixels[i] = static_cast<unsigned char>(componentValue);
			}
		}

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB8,
			m_width,
			m_height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			pixels.data());

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
