#include "Renderer.h"
using namespace Machi::Graphics;

std::shared_ptr<Machi::Geometry::Mesh> 
Renderer::get_mesh()
{
	return this->m_mesh[0];
}

Renderer&
Renderer::set_static_mesh(std::shared_ptr<Machi::Geometry::Mesh> m_mesh){

	this->m_mesh.push_back(m_mesh);

	return *this;
}
