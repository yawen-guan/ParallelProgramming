#include "../include/constant.hpp"
#include <iostream>

const int n = N;

int a[ n ][ n ], b[ n ][ n ], ans[ n ][ n ];

void readin() {
	FILE* f = fopen( "../data/matrix", "r" );

	for ( int i = 0; i < n; i++ )
		for ( int j = 0; j < n; j++ )
			fscanf( f, "%d", &a[ i ][ j ] );
	for ( int i = 0; i < n; i++ )
		for ( int j = 0; j < n; j++ )
			fscanf( f, "%d", &b[ i ][ j ] );

	fclose( f );
}

void mult() {
	for ( int i = 0; i < n; i++ )
		for ( int j = 0; j < n; j++ )
			for ( int k = 0; k < n; k++ )
				ans[ i ][ j ] += a[ i ][ k ] * b[ k ][ j ];
}

int main() {
	readin();
	mult();
	// for ( int i = 0; i < 10; i++ )
	// 	for ( int j = 0; j < 10; j++ )
	// 		printf( "%d ", ans[ i ][ j ] );
	return 0;
}