#include "model.h"

#include "../../utils/public/macros.h"
#include "triangle.h"
#include "color.h"
#include "shader.h"
#include "imageBuffer.h"

#include <array>

namespace helper
{
	void LoadObj( const AnsiChar* path, Model* model )
	{
		std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< float4 > temp_vertices;
		std::vector< float4 > temp_normals;

#pragma warning( disable : 4996 )

		FILE* file = fopen( path, "r" );

		SC_ASSERT( file != NULL, "Impossible to open the file !\n" );

		while ( 1 ) {

			char lineHeader[ 128 ];
			// read the first word of the line
			int res = fscanf( file, "%s", lineHeader );
			if ( res == EOF )
				break; // EOF = End Of File. Quit the loop.

					   // else : parse lineHeader
			if ( strcmp( lineHeader, "v" ) == 0 ) {
				float4 vertex;
				fscanf( file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
				temp_vertices.push_back( vertex );
			}
			else if ( strcmp( lineHeader, "vt" ) == 0 ) {
				Float u, v;
				fscanf( file, "%f %f\n", &u, &v );
			}
			else if ( strcmp( lineHeader, "vn" ) == 0 ) {
				float4 normal;
				fscanf( file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
				temp_normals.push_back( normal );
			}
			else if ( strcmp( lineHeader, "f" ) == 0 ) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[ 3 ], uvIndex[ 3 ], normalIndex[ 3 ];
				//int matches = fscanf( file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[ 0 ], &uvIndex[ 0 ], &normalIndex[ 0 ], &vertexIndex[ 1 ], &uvIndex[ 1 ], &normalIndex[ 1 ], &vertexIndex[ 2 ], &uvIndex[ 2 ], &normalIndex[ 2 ] );
				int matches = fscanf( file, "%d %d %d\n", &vertexIndex[ 0 ], &vertexIndex[ 1 ], &vertexIndex[ 2 ] );

				vertexIndices.push_back( vertexIndex[ 0 ] );
				vertexIndices.push_back( vertexIndex[ 1 ] );
				vertexIndices.push_back( vertexIndex[ 2 ] );
				uvIndices.push_back( uvIndex[ 0 ] );
				uvIndices.push_back( uvIndex[ 1 ] );
				uvIndices.push_back( uvIndex[ 2 ] );
				normalIndices.push_back( normalIndex[ 0 ] );
				normalIndices.push_back( normalIndex[ 1 ] );
				normalIndices.push_back( normalIndex[ 2 ] );
			}
		}

		// For each vertex of each triangle
		std::array< float4, 3 > verts;
		for ( unsigned int i = 0; i < vertexIndices.size(); i++ )
		{
			const Uint32 index = vertexIndices[ i ];
			verts[ i % 3 ] = temp_vertices[ index - 1 ];

			// every 3 verts
			if ( i % 3 == 2 )
			{
				model->AddTriangle( verts[ 2 ], verts[ 1 ], verts[ 0 ] );
			}
		}
	}
}

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
		Color( 0xffff0000 ),
		Color( 0xff00ff00 ),
		Color( 0xff0000ff ),
		Color( 0xffff00ff ),

		Color( 0xffffff00 ),
		Color( 0xff00ffff ),
		Color( 0xffffffff ),
		Color( 0xff000000 )
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
		triangle->ComputeNormal();

		result->m_triangles.push_back( triangle );
	}

	return result;
}

void Model::Load( const AnsiChar* path )
{
	helper::LoadObj( path, this );
}

void Model::Draw( ImageBuffer* buffer, Shader* shader ) const
{
	float4x4 rotate = float4x4::Identity();
	static float rot = 0.0f;
	rot += 1;

	rotate.SetRotation( rot, float4( 0, 1, 0 ).Normalized() );

	float4x4 translate = float4x4::Identity();
	translate.SetTranslation( m_position );
	const float4x4 transform = translate * rotate;

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