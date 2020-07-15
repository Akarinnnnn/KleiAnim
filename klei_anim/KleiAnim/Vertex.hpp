#pragma once
#include "Binary.hpp"

namespace KleiAnim::Vertex
{
	using Vertex = KleiAnim::Common::AlphaVertexNode;
	struct alignas(4) VertexGroup
	{
		Vertex v0;
		Vertex v1;
		Vertex v2;
		Vertex v3;
		Vertex v4;
		Vertex v5;
	};

	struct VertexInfo
	{
		float u1 = 0, u2 = 0, v1 = 0, v2 = 0;
		float x0 = 0, y0 = 0, x1 = 0, y1 = 0, z = 0;
		unsigned int atlasIndex = 0;
	};

	inline const VertexGroup* MakeGroup(const Vertex* vertices)
	{
		return reinterpret_cast<const VertexGroup*> (vertices);
	}

	inline const VertexGroup* MakeGroup(const std::array<Vertex, 6>& vertices)
	{
		return reinterpret_cast<const VertexGroup*> (vertices.data());
	}

	std::array<Vertex, 6> EXPORT_API VerticesFromInfo(const VertexInfo& info);
	std::array<Vertex, 6> EXPORT_API VerticesFromInfo(VertexInfo&& info);

	VertexInfo EXPORT_API FromGroup(const VertexGroup&);

	bool EXPORT_API VaildateGroup(const VertexGroup&);

	std::wstring ToString(VertexInfo& i)
	{
		std::wostringstream os;
#pragma warning(push)
#pragma warning (disable:26444)
		os.imbue(std::locale(""));
#pragma warning(pop)

		os << L"VertexInfo\n"
			<< L"atlas-" << i.atlasIndex << L'\n'
			<< L"x0,\ty0\t,x1\t,y1 = \n"
			<< i.x0 << L",\t" << i.y0 << L",\t" << i.x1 << L",\t" << i.y1 << L",\t\n"
			<< L"u1,\tu2,\tv1,\tv2 = \n"
			<< i.u1 << L",\t" << i.u2 << L",\t" << i.v1 << L",\t" << i.v2 << L",\t\n"
			<< L"z = " << i.z << std::endl;
		return os.str();
	}
}