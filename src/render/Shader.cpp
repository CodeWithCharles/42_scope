#include <glad/glad.h>

#include "render/Shader.hpp"

#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

namespace Scop
{
	Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
		: m_programId(0)
	{
		std::string vertexSource = loadFileContent(vertexPath);
		std::string fragmentSource = loadFileContent(fragmentPath);

		unsigned int vertexShaderId = 0;
		unsigned int fragmentShaderId = 0;

		try
		{
			vertexShaderId = compileShader(GL_VERTEX_SHADER, vertexSource);
			fragmentShaderId = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
			linkProgram(vertexShaderId, fragmentShaderId);
		}
		catch (...)
		{
			if (vertexShaderId != 0)
				glDeleteShader(vertexShaderId);
			if (fragmentShaderId != 0)
				glDeleteShader(fragmentShaderId);
			throw;
		}
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}

	Shader::~Shader()
	{
		if (m_programId != 0)
		{
			glDeleteProgram(m_programId);
			m_programId = 0;
		}
	}

	void	Shader::use() const
	{
		glUseProgram(m_programId);
	}

	void	Shader::setMat4(const std::string& uniformName, const Math::Mat4& matrix) const
	{
		int location = glGetUniformLocation(m_programId, uniformName.c_str());
		if (location == -1)
			return;

		glUniformMatrix4fv(location, 1, GL_FALSE, matrix.raw());
	}

	void	Shader::setInt(const std::string& uniformName, int value) const
	{
		int location = glGetUniformLocation(m_programId, uniformName.c_str());
		if (location == -1)
			return;
		glUniform1i(location, value);
	}

	void	Shader::setFloat(const std::string& uniformName, float value) const
	{
		int location = glGetUniformLocation(m_programId, uniformName.c_str());
		if (location == -1)
			return;

		glUniform1f(location, value);
	}

	unsigned int	Shader::getProgramId() const
	{
		return m_programId;
	}

	unsigned int	Shader::compileShader(unsigned int shaderType, const std::string &source)
	{
		unsigned int shaderId = glCreateShader(shaderType);
		if (shaderId == 0)
			throw std::runtime_error("Failed to create shader");

		const char *shaderSource = source.c_str();
		glShaderSource(shaderId, 1, &shaderSource, nullptr);
		glCompileShader(shaderId);

		int success = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			int logLength = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);

			std::vector<char> log(logLength > 0 ? logLength : 1);
			glGetShaderInfoLog(shaderId, static_cast<int>(log.size()), nullptr, log.data());

			glDeleteShader(shaderId);
			throw std::runtime_error(std::string("Shader compilation failed: ") + log.data());
		}

		return shaderId;
	}

	void	Shader::linkProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId)
	{
		unsigned int programId = glCreateProgram();
		if (programId == 0)
			throw std::runtime_error("Failed to create shader program");

		glAttachShader(programId, vertexShaderId);
		glAttachShader(programId, fragmentShaderId);
		glLinkProgram(programId);

		int success = 0;
		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			int logLength = 0;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);

			std::vector<char> log(logLength > 0 ? logLength : 1);
			glGetProgramInfoLog(programId, static_cast<int>(log.size()), nullptr, log.data());

			glDeleteProgram(programId);
			throw std::runtime_error(std::string("Shader linking failed: ") + log.data());
		}

		m_programId = programId;
	}

	std::string	Shader::loadFileContent(const std::string& path) const
	{
		std::ifstream file(path.c_str());
		if (!file.is_open())
			throw std::runtime_error("Failed to open shader file: " + path);

		std::ostringstream buffer;
		buffer << file.rdbuf();

		return buffer.str();
	}
}
