#pragma once

#include "Vertex.hpp"
#include "math/Vec3.hpp"

#include <vector>

namespace Scope
{
	class Mesh
	{
		private:
			std::vector<Vertex>			m_vertices;
			std::vector<unsigned int>	m_indices;

			unsigned int				m_vao;
			unsigned int				m_vbo;
			unsigned int				m_ebo;

			Math::Vec3					m_minBounds;
			Math::Vec3					m_maxBounds;
			Math::Vec3					m_center;

		public:
			Mesh(const std::vector<Vertex>& vertices,
				const std::vector<unsigned int>& indices);
			~Mesh();

			Mesh(const Mesh&) = delete;
			Mesh& operator=(const Mesh&) = delete;

			Mesh(Mesh&&) = delete;
			Mesh& operator=(Mesh&&) = delete;

			void	draw() const;

			const std::vector<Vertex>&			getVertices() const;
			const std::vector<unsigned int>&	getIndices() const;

			const Math::Vec3&	getMinBounds() const;
			const Math::Vec3&	getMaxBounds() const;
			const Math::Vec3&	getCenter() const;
			Math::Vec3			getSize() const;

		private:
			void	setupMesh();
			bool	hasIndices() const;
			void	computeBounds();
	};
}
