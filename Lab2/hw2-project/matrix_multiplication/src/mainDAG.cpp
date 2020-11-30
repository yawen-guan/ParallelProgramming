#include "../include/constant.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

int** mult( int** a, int** b, int n, int l, int m ) {
	int** c = new int*[ n ];
	for ( int i = 0; i < n; i++ )
		c[ i ] = new int[ m ];

	for ( int i = 0; i < n; i++ ) {
		for ( int j = 0; j < m; j++ ) {
			c[ i ][ j ] = 0;
			for ( int k = 0; k < l; k++ ) {
				c[ i ][ j ] += a[ i ][ k ] * b[ k ][ j ];
			}
		}
	}
	return c;
}

int**** divide4( int** a, int adjN, int adjL, int n, int l ) {
	int**** as = new int***[ 2 ];
	for ( int x = 0; x < 2; x++ ) {
		as[ x ] = new int**[ 2 ];
		for ( int y = 0; y < 2; y++ ) {
			as[ x ][ y ] = new int*[ adjN ];
			for ( int i = 0; i < adjN; i++ ) {
				as[ x ][ y ][ i ] = new int[ adjL ];
				for ( int j = 0; j < adjL; j++ ) {
					int I = i + ( x & 1 ) * adjN;
					int J = j + ( y & 1 ) * adjL;
					as[ x ][ y ][ i ][ j ] = ( I < n && J < l ) ? a[ I ][ J ] : 0;
				}
			}
		}
	}
	return as;
}

void del_ab( int**** as, int adjN ) {
	for ( int x = 0; x < 2; x++ ) {
		for ( int y = 0; y < 2; y++ ) {
			for ( int i = 0; i < adjN; i++ ) {
				delete[] as[ x ][ y ][ i ];
			}
			delete[] as[ x ][ y ];
		}
		delete[] as[ x ];
	}
	delete[] as;
}

void del_s( int*** s, int adjN ) {
	for ( int i = 0; i < 8; i++ ) {
		for ( int j = 0; j < adjN; j++ ) {
			delete[] s[ i ][ j ];
		}
		delete[] s[ i ];
	}
	delete[] s;
}

int** multDAG( int** a, int** b, int n, int l, int m ) {
	if ( n == 1 || l == 1 || m == 1 ) return mult( a, b, n, l, m );

	int** c = new int*[ n ];
	for ( int i = 0; i < n; i++ )
		c[ i ] = new int[ m ];

	int adjN = ( n >> 1 ) + ( n & 1 );
	int adjL = ( l >> 1 ) + ( l & 1 );
	int adjM = ( m >> 1 ) + ( m & 1 );

	int**** as = divide4( a, adjN, adjL, n, l );
	int**** bs = divide4( b, adjL, adjM, l, m );

	int*** s = new int**[ 8 ];
	s[ 0 ] = multDAG( as[ 0 ][ 0 ], bs[ 0 ][ 0 ], adjN, adjL, adjM );
	s[ 1 ] = multDAG( as[ 0 ][ 1 ], bs[ 1 ][ 0 ], adjN, adjL, adjM );
	s[ 2 ] = multDAG( as[ 0 ][ 0 ], bs[ 0 ][ 1 ], adjN, adjL, adjM );
	s[ 3 ] = multDAG( as[ 0 ][ 1 ], bs[ 1 ][ 1 ], adjN, adjL, adjM );
	s[ 4 ] = multDAG( as[ 1 ][ 0 ], bs[ 0 ][ 0 ], adjN, adjL, adjM );
	s[ 5 ] = multDAG( as[ 1 ][ 1 ], bs[ 1 ][ 0 ], adjN, adjL, adjM );
	s[ 6 ] = multDAG( as[ 1 ][ 0 ], bs[ 0 ][ 1 ], adjN, adjL, adjM );
	s[ 7 ] = multDAG( as[ 1 ][ 1 ], bs[ 1 ][ 1 ], adjN, adjL, adjM );

	for ( int i = 0; i < adjN; i++ ) {
		for ( int j = 0; j < adjM; j++ ) {
			c[ i ][ j ] = s[ 0 ][ i ][ j ] + s[ 1 ][ i ][ j ];
			if ( j + adjM < m ) c[ i ][ j + adjM ] = s[ 2 ][ i ][ j ] + s[ 3 ][ i ][ j ];
			if ( i + adjN < n ) c[ i + adjN ][ j ] = s[ 4 ][ i ][ j ] + s[ 5 ][ i ][ j ];
			if ( i + adjN < n && j + adjM < m ) c[ i + adjN ][ j + adjM ] = s[ 6 ][ i ][ j ] + s[ 7 ][ i ][ j ];
		}
	}

	del_ab( as, adjN );
	del_ab( bs, adjL );
	del_s( s, adjN );

	return c;
}

void readin( int** a, int** b, int n ) {
	FILE* f = fopen( "../data/matrix", "r" );
	for ( int i = 0; i < n; i++ )
		for ( int j = 0; j < n; j++ )
			fscanf( f, "%d", &a[ i ][ j ] );
	for ( int i = 0; i < n; i++ )
		for ( int j = 0; j < n; j++ )
			fscanf( f, "%d", &b[ i ][ j ] );
	fclose( f );
}

int main() {
	int n = N;

	int **a, **b;
	a = new int*[ n ];
	b = new int*[ n ];
	for ( int i = 0; i < n; i++ ) {
		a[ i ] = new int[ n ];
		b[ i ] = new int[ n ];
	}
	readin( a, b, n );

	int** ans = multDAG( a, b, n, n, n );
	// for ( int i = 0; i < 10; i++ ) {
	// 	for ( int j = 0; j < 10; j++ ) {
	// 		printf( "%d ", ans[ i ][ j ] );
	// 	}
	// }

	return 0;
}