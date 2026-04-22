#pragma once

#include <string>
#include <vector>

namespace Scop
{
	class Texture
	{
		private:
			unsigned int	m_id;
			int				m_width;
			int				m_height;

		public:
			Texture(const std::string& path);
			~Texture();

			Texture(const Texture&) = delete;
			Texture& operator=(const Texture&) = delete;

			Texture(Texture&&) = delete;
			Texture& operator=(Texture&&) = delete;

			void	bind() const;

			int		getWidth() const;
			int		getHeight() const;

		private:
			void	loadFromPpm(const std::string& path);
	};
}
