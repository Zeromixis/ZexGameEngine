#ifndef _TEST_LIGHTNING_TEST_H_
#define _TEST_LIGHTNING_TEST_H_

#include <vector>
#include "Math/Vector.h"
#include "../Timer.h"
#include "Core/ArrayBuffer.h"

namespace ZGE
{
	class LightningTest
	{
	public:
		LightningTest ()
			: m_Timer ( false )
		{
			srand ( time ( nullptr ) );
			m_Points.reserve ( 300 );
			GenerateRandomPoints ();
			glGenVertexArrays ( 1, &m_VertexArray );
			glBindVertexArray ( m_VertexArray );

			F32 *positionData = new F32[ m_Points.size () * 3 ];
			for ( size_t i = 0; i < m_Points.size (); ++i )
			{
				positionData[ i * 3 ] = m_Points[ i ].x ();
				positionData[ i * 3 + 1 ] = m_Points[ i ].y ();
				positionData[ i * 3 + 2 ] = m_Points[ i ].z ();
			}

			
			m_pVertexBuffer = new ArrayBuffer ( m_Points.size () * 3 * 4, ArrayBuffer::ArrayBufferUsage::Static );
			m_pVertexBuffer->CopyData ( positionData, m_Points.size () * 3 * 4 );
			m_pVertexBuffer->TransferData ();
			

			// Bind Tex Data
			F32 *texData = new F32[ m_Points.size () * 2 ];
			for ( size_t i = 0; i < m_Points.size (); ++i )
			{
				if ( i % 2 == 0 )
				{
					texData[ i * 2 ] = 0.0f;
				}
				else
				{
					texData[ i * 2 ] = 1.0f;
				}
				texData[ i * 2 + 1 ] = 0.0f;
			}

			m_pTexCoordBuffer = new ArrayBuffer ( m_Points.size () * 2 * 4, ArrayBuffer::ArrayBufferUsage::Static );
			m_pTexCoordBuffer->CopyData ( texData, m_Points.size () * 2 * 4 );
			m_pTexCoordBuffer->TransferData ();

			/*
			glGenBuffers ( 1, &m_TexCoordBuffer );
			glBindBuffer ( GL_ARRAY_BUFFER, m_TexCoordBuffer );
			glBufferData ( GL_ARRAY_BUFFER, m_Points.size () * 2 * 4, texData, GL_STATIC_DRAW );
			*/

			glBindBuffer ( GL_ARRAY_BUFFER, m_pVertexBuffer->BufferId () );
			glEnableVertexAttribArray ( 0 );
			glVertexAttribPointer
			(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				0
			);

			glBindBuffer ( GL_ARRAY_BUFFER, m_pTexCoordBuffer->BufferId () );
			glEnableVertexAttribArray ( 1 );
			glVertexAttribPointer
			(
				1,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				0
			);


			glGenTextures ( 1, &m_LightingTexture );
			glBindTexture ( GL_TEXTURE_1D, m_LightingTexture );
			glTexParameteri ( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri ( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri ( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

			unsigned char buf[ 32 * 4 ] = { 0 };
			size_t i = 0;
			for ( i = 0; i < sizeof ( buf ) / 2; i += 4 )
			{
				buf[ i + 0 ] = 16 * i / 4; 
				buf[ i + 1 ] = 16 * i / 4;
				buf[ i + 2 ] = 255;
				buf[ i + 3 ] = 255;
			}
			for ( i = 0 ; i < sizeof ( buf ) / 2; i += 4 )
			{
				buf[ i + 64 + 0 ] = 240 - 16 * i / 4;
				buf[ i + 64 + 1 ] = 240 - 16 * i / 4;
				buf[ i + 64 + 2 ] = 255;
				buf[ i + 64 + 3 ] = 255;
			}
			glTexImage1D ( GL_TEXTURE_1D, 0, 4, sizeof ( buf ) / 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf );

			m_Timer.Start ();
			m_NowTime = m_Timer.GetElapsedTimeMS ();

			isFirst = false;
		}

		~LightningTest ()
		{
			delete m_pVertexBuffer;
			glDeleteTextures ( 1, &m_LightingTexture );
		}
		
		void GenerateRandomPoints ()
		{
			Interpolation ( BEGINPOINT, ENDPOINT );
		}

		void Interpolation ( const Vector3f &begin, const Vector3f &end )
		{
			F32 len = Length ( end - begin );
			if ( len < 0.6f )
			{
				// Enough
				m_Points.push_back ( begin );
				m_Points.push_back ( end );
				return;
			}
			else
			{
				int randNumber = 0;

				// The jitter.
				double mid = ( rand () % 4 + 3 ) / 10.0f;
				Vector3f newPoint = begin + ( end - begin ) * mid;

				// Give offset in Y axis
				randNumber = rand () % 2;
				Vector3f vec = end - begin;
				Vector3f verticalVec;
				if ( true == randNumber )
				{
					verticalVec = Vector3f ( vec.y (), -vec.x (), vec.z () );
				}
				else
				{
					verticalVec = Vector3f ( -vec.y (), vec.x (), vec.z () );
				}
				verticalVec = Normalize ( verticalVec );
				mid = ( rand () % 2 + 1 ) / 20.0f;
				verticalVec *= mid;
				newPoint += verticalVec;


				Interpolation ( begin, newPoint );

				randNumber = rand () % 3;
				// The Fork.
				if ( randNumber == 2 )
				{
					//Vector3f _beginPoint = begin;
					//Vector3f _endPoint = newPoint;

					double mid = ( rand () % 5 + 3 ) / 10.0f;
					Vector3f forkPoint = begin + Normalize ( end - begin ) * mid;
					Vector3f forkPoint2 = forkPoint + Normalize ( end - begin ) * 0.2f;
					Vector3f vec = end - begin;
					Vector3f verticalVec;
					randNumber = rand () % 2;
					if ( true == randNumber )
					{
						verticalVec = Vector3f ( vec.y (), -vec.x (), vec.z () );
					}
					else
					{
						verticalVec = Vector3f ( -vec.y (), vec.x (), vec.z () );
					}
					verticalVec = Normalize ( verticalVec );
					mid = ( rand () % 2 + 1 ) / 20.0f;
					verticalVec *= mid;
					forkPoint2 += verticalVec;
					m_Points.push_back ( forkPoint );
					m_Points.push_back ( forkPoint2 );
					//Interpolation ( forkPoint, forkPoint2 );
				}


				Interpolation ( newPoint, end );

// 				randNumber = rand () % 3;
// 				if ( randNumber == 2 )
// 				{
// 					Vector3f _beginPoint = newPoint;
// 					Vector3f _endPoint = end;
// 
// 					double mid = ( rand () % 5 + 3 ) / 10.0f;
// 					Vector3f forkPoint = _beginPoint + Normalize ( _endPoint - _beginPoint ) * mid;
// 					Vector3f forkPoint2 = forkPoint + Normalize ( _endPoint - _beginPoint ) * 0.2f;
// 					Vector3f vec = end - begin;
// 					Vector3f verticalVec;
// 					if ( true == randNumber )
// 					{
// 						verticalVec = Vector3f ( vec.y (), -vec.x (), vec.z () );
// 					}
// 					else
// 					{
// 						verticalVec = Vector3f ( -vec.y (), vec.x (), vec.z () );
// 					}
// 					verticalVec = Normalize ( verticalVec );
// 					mid = ( rand () % 2 + 1 ) / 20.0f;
// 					verticalVec *= mid;
// 					forkPoint2 += verticalVec;
// 					Interpolation ( forkPoint, forkPoint2 );
// 				}




			}
		}

		void Draw ()
		{
			if ( !isFirst )
			{
				m_Timer.Start ();
				m_NowTime = m_Timer.GetElapsedTimeMS ();
				isFirst = true;
			}

			// After 3 second start draw
			if ( m_Timer.GetElapsedTimeMS () - m_NowTime > 3000.0f && m_Timer.GetElapsedTimeMS() - m_NowTime < 6000.0f )
			{
				// Use divided number to control speed
				F32 progress = ( m_Timer.GetElapsedTimeMS () - m_NowTime - 3000.0f ) / 1450;
				//std::cout << progress << std::endl;

				// Some progress control code
				int tailIndex = 0;
				int headIndex = std::min< int > ( ( int ) ( m_Points.size () * ( progress / 2 ) ), m_Points.size () );
				if ( headIndex == 0 )
					headIndex = 1;
				std::vector < int > indices;
				for ( int i = tailIndex; i < headIndex; ++i )
				{
					indices.push_back ( i );
				}
				if ( indices.size () % 2 != 0 )
				{
					indices.push_back ( indices[ indices.size () - 1 ] + 1 );
				}

				// Draw
				glBindTexture ( GL_TEXTURE_1D, m_LightingTexture );
				glLineWidth ( 4 );
				//glDrawArrays ( GL_LINES, 0, m_Points.size () );
				glDrawElements ( GL_LINES, indices.size (), GL_UNSIGNED_INT, &indices[ 0 ] );
				glLineWidth ( 1 );
			}
			else if ( m_Timer.GetElapsedTimeMS () - m_NowTime >= 6000.0f )
			{
				m_NowTime = m_Timer.GetElapsedTimeMS ();
			}

		}

		bool isFirst;
		std::vector< Vector3f > m_Points;
		GLuint m_VertexArray;
		ArrayBuffer *m_pVertexBuffer;
		ArrayBuffer *m_pTexCoordBuffer;
		
		
		GLuint m_LightingTexture;

		Vector3f BEGINPOINT = Vector3f ( -3.0f, 0.0f, -0.5f );
		Vector3f ENDPOINT = Vector3f ( 3.0f, 0.0f, -0.5f );
		F64 m_NowTime;
		Timer m_Timer;
		

	};
}

#endif