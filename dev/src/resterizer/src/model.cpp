#include "model.h"
#include "triangle.h"
#include "color.h"
#include "shader.h"
#include "imageBuffer.h"

Model::Model()
	: m_transform( float4x4::Identity() )
{
}

Model::~Model()
{
	for ( Uint32 i = 0; i < m_triangles.size(); ++i )
	{
		delete m_triangles[ i ];
	}

	m_triangles.clear();
}

Model* Model::CreateCube( const float4& pos, const float4& size )
{
	static const float4 oneCube[ 8 ] = {
		{ 0, 0, 0, 1 },
		{ 1, 0, 0, 1 },
		{ 1, 0, 1, 1 },
		{ 0, 0, 1, 1 },

		{ 0, 1, 0, 1 },
		{ 1, 1, 0, 1 },
		{ 1, 1, 1, 1 },
		{ 0, 1, 1, 1 }
	};

	static const Color colors[ 8 ] =
	{
		0xffff0000,
		0xff00ff00,
		0xff0000ff,
		0xffff00ff,

		0xffffff00,
		0xff00ffff,
		0xffffffff,
		0xff000000
	};

	static const Uint8 indices[ 36 ] =
	{
		// front
		5, 0, 1,
		0, 5, 4,

		// back
		2, 3, 6,
		7, 6, 3,

		// top
		4, 6, 7,
		4, 5, 6,

		// bottom
		0, 2, 1,
		0, 3, 2,

		// left
		0, 4, 7,
		0, 7, 3,

		// right
		1, 2, 6,
		1, 6, 5
	};


	float4 cube[ 8 ];
	for ( Uint32 i = 0; i < 8; ++i )
	{
		cube[ i ].x = oneCube[ i ].x * size.x;
		cube[ i ].y = oneCube[ i ].y * size.y;
		cube[ i ].z = oneCube[ i ].z * size.z;

		cube[ i ] += pos;
		cube[ i ].w = 1;
	}

	Model* result = new Model();
	
	const Uint32 faces = 6;
	const Uint32 tris = faces * 2;
	for ( Uint32 i = 0; i < tris; ++i )
	{
		CTriangle* triangle = new CTriangle(
			cube[ indices[ i * 3 ] ],
			cube[ indices[ i * 3 + 1 ] ],
			cube[ indices[ i * 3 + 2 ] ] );

		triangle->SetVertexColor( 0, colors[ indices[ i * 3 ] ] );
		triangle->SetVertexColor( 1, colors[ indices[ i * 3 + 1 ] ] );
		triangle->SetVertexColor( 2, colors[ indices[ i * 3 + 2 ] ] );

		result->m_triangles.push_back( triangle );
	}

	//CTriangle* triangle = new CTriangle(
	//	{ 1, 1, 0, 1 },
	//	{ 0, 0, 0, 1 },
	//	{ 1, 0, 0, 1 } );

	//triangle->SetVertexColor( 0, 0xffff0000 );
	//triangle->SetVertexColor( 1, 0xff00ff00 );
	//triangle->SetVertexColor( 2, 0xff0000ff );

	//result->m_triangles.push_back( triangle );

	return result;
}

void Model::Draw( ImageBuffer* buffer, Shader* shader ) const
{
	float4x4 rotate = float4x4::Identity();
	static float rot = 0.0f;
	rot += 2;

	rotate.SetRotation( rot, float4( 0, 1, 0 ).Normalized() );

	float4x4 translate = float4x4::Identity();
	translate.SetTranslation( { 0, 0.1f, -8 } );
	float4x4 transform = translate * rotate;

	shader->SetModelMatrix( transform );

	for ( Uint32 i = 0; i < m_triangles.size(); ++i )
	{
		m_triangles[ i ]->Draw( buffer, shader );
	}
}

void Model::AddTriangle( const float4& a, const float4& b, const float4& c )
{
	CTriangle* tri = new CTriangle();
	tri->SetVertexPos( 0, a );
	tri->SetVertexColor( 0, Color::Red() );

	tri->SetVertexPos( 1, b );
	tri->SetVertexColor( 1, Color::Green() );

	tri->SetVertexPos( 2, c );
	tri->SetVertexColor( 2, Color::Blue() );

	m_triangles.push_back( tri );
}