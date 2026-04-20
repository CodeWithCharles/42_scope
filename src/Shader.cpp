#include <glad/glad.h>

#include "Shader.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace Scope
{
	Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource)
		: m_programId(0)
	{
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

	void Shader::use() const
	{
		glUseProgram(m_programId);
	}

	unsigned int Shader::getProgramId() const
	{
		return m_programId;
	}

	unsigned int Shader::compileShader(unsigned int shaderType, const std::string &source)
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

	void Shader::linkProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId)
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
}
