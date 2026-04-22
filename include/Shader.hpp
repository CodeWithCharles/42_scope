#pragma once

#include "math/Mat4.hpp"
#include <string>

namespace Scop
{
	class Shader
	{
		private:
			unsigned int	m_programId;

		public:
			Shader(const std::string& vertexPath, const std::string& fragmentPath);
			~Shader();

			Shader(const Shader&) = delete;
			Shader& operator=(const Shader&) = delete;

			Shader(Shader&&) = delete;
			Shader& operator=(Shader&&) = delete;

			void			use() const;
			void			setMat4(const std::string& uniformName, const Math::Mat4& matrix) const;
			void			setInt(const std::string& uniformName, int value) const;
			void			setFloat(const std::string& uniformName, float value) const;
			unsigned int	getProgramId() const;

		private:
			unsigned int	compileShader(unsigned int shaderType, const std::string& source);
			void			linkProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId);

			std::string		loadFileContent(const std::string& path) const;
	};
}
