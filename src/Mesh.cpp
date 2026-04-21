#include <glad/glad.h>

#include "Mesh.hpp"

#include <cstddef>

namespace Scope
{
	Mesh::Mesh(const std::vector<Vertex>& vertices,
		const std::vector<unsigned int>& indices)
		:	m_vertices(vertices),
			m_indices(indices),
			m_vao(0),
			m_vbo(0),
			m_ebo(0)
	{
		setupMesh();
	}

	Mesh::~Mesh()
	{
		if (m_ebo != 0)
		{
			glDeleteBuffers(1, &m_ebo);
			m_ebo = 0;
		}

		if (m_vbo != 0)
		{
			glDeleteBuffers(1, &m_vbo);
			m_vbo = 0;
		}

		if (m_vao != 0)
		{
			glDeleteVertexArrays(1, &m_vao);
			m_vao = 0;
		}
	}

	void	Mesh::draw() const
	{
		glBindVertexArray(m_vao);

		if (hasIndices())
		{
			glDrawElements(
				GL_TRIANGLES,
				static_cast<int>(m_indices.size()),
				GL_UNSIGNED_INT,
				nullptr);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(m_vertices.size()));
		}

		glBindVertexArray(0);
	}

	const std::vector<Vertex>&	Mesh::getVertices() const
	{
		return m_vertices;
	}

	const std::vector<unsigned int>&	Mesh::getIndices() const
	{
		return m_indices;
	}

	bool	Mesh::hasIndices() const
	{
		return !m_indices.empty();
	}

	void	Mesh::setupMesh()
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		if (hasIndices())
			glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(
			GL_ARRAY_BUFFER,
			static_cast<long>(m_vertices.size() * sizeof(Vertex)),
			m_vertices.data(),
			GL_STATIC_DRAW);

		if (hasIndices())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				static_cast<long>(m_indices.size() * sizeof(unsigned int)),
				m_indices.data(),
				GL_STATIC_DRAW);
		}

		glVertexAttribPointer(
			Vertex::POSITION_LOCATION,
			Vertex::POSITION_COMPONENT_COUNT,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, position)));
		glEnableVertexAttribArray(Vertex::POSITION_LOCATION);

		glVertexAttribPointer(
			Vertex::COLOR_LOCATION,
			Vertex::COLOR_COMPONENT_COUNT,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, color)));
		glEnableVertexAttribArray(Vertex::COLOR_LOCATION);

		glVertexAttribPointer(
			Vertex::UV_LOCATION,
			Vertex::UV_COMPONENT_COUNT,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, uv)));
		glEnableVertexAttribArray(Vertex::UV_LOCATION);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}
