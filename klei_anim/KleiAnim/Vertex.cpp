#include "../pch.h"
#include "Vertex.hpp"
using namespace KleiAnim::Vertex;

VertexInfo EXPORT_API KleiAnim::Vertex::FromGroup(const VertexGroup& group)
{
	VertexInfo ret;

	auto f = group.v0.style.structure.f;
	ret.atlasIndex = roundf(f);

	ret.x0 = group.v0.style.structure.a;
	ret.x1 = group.v2.style.structure.a;

	ret.y0 = group.v2.style.structure.b;
	ret.y1 = group.v0.style.structure.b;

	ret.u1 = group.v0.style.structure.d;
	ret.u2 = group.v1.style.structure.d;

	ret.v1 = group.v0.style.structure.e;
	ret.v2 = group.v2.style.structure.e;

	ret.z = group.v0.style.structure.c;

	return ret;
}

//祖传代码？
//不敢动，不敢动
bool EXPORT_API KleiAnim::Vertex::VaildateGroup(const VertexGroup& group)
{
	union ___X
	{
		VertexGroup g;
		float arr[6][6];
	} *u =  (___X*)&group;

	if (u->arr[0][0] != u->arr[2][0] || u->arr[2][0] != u->arr[5][0]) //x0
		return false;

	if (u->arr[1][0] != u->arr[3][0] || u->arr[3][0] != u->arr[4][0]) //x1
		return false;

	if (u->arr[0][1] != u->arr[1][1] || u->arr[1][1] != u->arr[3][1]) //y0
		return false;

	if (u->arr[2][1] != u->arr[4][1] || u->arr[4][1] != u->arr[5][1]) //y1
		return false;

	if (u->arr[0][3] != u->arr[2][3] || u->arr[2][3] != u->arr[5][3]) //u1
		return false;

	if (u->arr[1][3] != u->arr[3][3] || u->arr[3][3] != u->arr[4][3]) //u2
		return false;

	if (u->arr[0][4] != u->arr[1][4] || u->arr[1][4] != u->arr[3][4]) //v1
		return false;

	if (u->arr[2][4] != u->arr[4][4] || u ->arr[4][4] != u->arr[3][5]) //v2
		return false;

	float prev = u->arr[0][2]; //z
	for (auto* v : u->arr)
		if (v[2] != prev)
			return false;
		else
			prev = v[2];

	prev = u->arr[0][5]; //sampler
	for (auto* v : u->arr)
		if (v[5] != prev)
			return false;
		else
			prev = v[5];

	return true;
}
