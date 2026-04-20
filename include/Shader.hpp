#pragma once

#include <string>

namespace Scope
{
	class Shader
	{
		private:
			unsigned int	m_programId;

		public:
			Shader(const std::string& vertexSource, const std::string& fragmentSource);
			~Shader();

			Shader(const Shader&) = delete;
			Shader& operator=(const Shader&) = delete;

			Shader(Shader&&) = delete;
			Shader& operator=(Shader&&) = delete;

			void			use() const;
			unsigned int	getProgramId() const;

		private:
			unsigned int	compileShader(unsigned int shaderType, const std::string& source);
			void			linkProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId);
	};
}
