#include "../include/constant.hpp"
#include "boost/shared_array.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

typedef boost::shared_array< boost::shared_array< int > > SA;
int a[ N ][ N ], b[ N ][ N ];

struct matrix {
	int x, y;  // top-left corner
	matrix( int x = 0, int y = 0 ) : x( x ), y( y ) {}
	void def( int x, int y ) {
		this->x = x;
		this->y = y;
	}
	void print() {
		printf( "matrix (%d, %d)\n", x, y );
	}
};

void readin( int n ) {
	FILE* f = fopen( "../data/matrix", "r" );

	for ( int i = 0; i < n; i++ )
		for ( int j = 0; j < n; j++ )
			fscanf( f, "%d", &a[ i ][ j ] );
	for ( int i = 0; i < n; i++ )
		for ( int j = 0; j < n; j++ )
			fscanf( f, "%d", &b[ i ][ j ] );

	fclose( f );
}

void divide4( matrix& u, matrix& u11, matrix& u12, matrix& u21, matrix& u22, int n ) {
	int umidx = ( u.x + u.x + n - 1 ) / 2;
	int umidy = ( u.y + u.y + n - 1 ) / 2;
	u11.def( u.x, u.y );
	u12.def( u.x, umidy + 1 );
	u21.def( umidx + 1, u.y );
	u22.def( umidx + 1, umidy + 1 );
}

void add( SA res, SA u, SA v, int x, int y, int n ) {
	for ( int i = 0; i < n; i++ )
		for ( int j = 0; j < n; j++ )
			res[ x + i ][ y + j ] = u[ i ][ j ] + v[ i ][ j ];
}

SA mult( matrix u, matrix v, int n ) {
	SA res( new boost::shared_array< int >[ n ] );
	for ( int i = 0; i < n; i++ )
		res[ i ].reset( new int[ n ] );

	if ( n == 2 ) {
		for ( int i = 0; i < 2; i++ )
			for ( int j = 0; j < 2; j++ ) {
				res[ i ][ j ] = 0;
				for ( int k = 0; k < 2; k++ )
					res[ i ][ j ] += a[ u.x + i ][ u.y + k ] * b[ v.x + k ][ v.y + j ];
			}
	}
	else {
		matrix u11, u12, u21, u22, v11, v12, v21, v22;
		divide4( u, u11, u12, u21, u22, n );
		divide4( v, v11, v12, v21, v22, n );

		int mid = ( n - 1 ) / 2 + 1;
		add( res, mult( u11, v11, n / 2 ), mult( u12, v21, n / 2 ), 0, 0, n / 2 );		// 11
		add( res, mult( u11, v12, n / 2 ), mult( u12, v22, n / 2 ), 0, mid, n / 2 );	// 12
		add( res, mult( u21, v11, n / 2 ), mult( u22, v21, n / 2 ), mid, 0, n / 2 );	// 21
		add( res, mult( u21, v12, n / 2 ), mult( u22, v22, n / 2 ), mid, mid, n / 2 );	// 22
	}
	return res;
}

int main() {
	int n = N;
	readin( n );
	auto ans = mult( matrix( 0, 0 ), matrix( 0, 0 ), n );
	// for ( int i = 0; i < n; i++ )
	// 	for ( int j = 0; j < n; j++ )
	// 		printf( "%d ", ans[ i ][ j ] );
	return 0;
}