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

int** add_and_minus( int** u, int** v, int n, int l, int flag ) {
	int** sum = new int*[ n ];
	for ( int i = 0; i < n; i++ ) {
		sum[ i ] = new int[ l ];
		for ( int j = 0; j < l; j++ ) {
			sum[ i ][ j ] = u[ i ][ j ] + v[ i ][ j ] * flag;
		}
	}
	return sum;
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

void del_s( int*** s, int adjL, int adjN ) {
	for ( int i = 0; i < 10; i++ ) {
		if ( i == 0 || i == 3 || i == 5 || i == 7 || i == 9 ) {
			for ( int j = 0; j < adjL; j++ ) {
				delete[] s[ i ][ j ];
			}
		}
		else {
			for ( int j = 0; j < adjN; j++ ) {
				delete[] s[ i ][ j ];
			}
		}
		break;
		delete[] s[ i ];
	}
	delete[] s;
}

void del_M( int*** M, int n ) {
	for ( int i = 0; i < 7; i++ ) {
		for ( int j = 0; j < ( n >> 1 ); j++ ) {
			delete[] M[ i ][ j ];
		}
		delete[] M[ i ];
	}
	delete[] M;
}

int** Strassen( int** a, int** b, int n, int l, int m ) {
	if ( n == 1 || l == 1 || m == 1 ) return mult( a, b, n, l, m );

	int** c = new int*[ n ];
	for ( int i = 0; i < n; i++ )
		c[ i ] = new int[ m ];

	int adjN = ( n >> 1 ) + ( n & 1 );
	int adjL = ( l >> 1 ) + ( l & 1 );
	int adjM = ( m >> 1 ) + ( m & 1 );

	int**** as = divide4( a, adjN, adjL, n, l );
	int**** bs = divide4( b, adjL, adjM, l, m );

	int*** s = new int**[ 10 ];
	s[ 0 ] = add_and_minus( bs[ 0 ][ 1 ], bs[ 1 ][ 1 ], adjL, adjM, -1 );
	s[ 1 ] = add_and_minus( as[ 0 ][ 0 ], as[ 0 ][ 1 ], adjN, adjL, 1 );
	s[ 2 ] = add_and_minus( as[ 1 ][ 0 ], as[ 1 ][ 1 ], adjN, adjL, 1 );
	s[ 3 ] = add_and_minus( bs[ 1 ][ 0 ], bs[ 0 ][ 0 ], adjL, adjM, -1 );
	s[ 4 ] = add_and_minus( as[ 0 ][ 0 ], as[ 1 ][ 1 ], adjN, adjL, 1 );
	s[ 5 ] = add_and_minus( bs[ 0 ][ 0 ], bs[ 1 ][ 1 ], adjL, adjM, 1 );
	s[ 6 ] = add_and_minus( as[ 0 ][ 1 ], as[ 1 ][ 1 ], adjN, adjL, -1 );
	s[ 7 ] = add_and_minus( bs[ 1 ][ 0 ], bs[ 1 ][ 1 ], adjL, adjM, 1 );
	s[ 8 ] = add_and_minus( as[ 0 ][ 0 ], as[ 1 ][ 0 ], adjN, adjL, -1 );
	s[ 9 ] = add_and_minus( bs[ 0 ][ 0 ], bs[ 0 ][ 1 ], adjL, adjM, 1 );

	int*** M = new int**[ 7 ];
	M[ 0 ] = Strassen( as[ 0 ][ 0 ], s[ 0 ], adjN, adjL, adjM );
	M[ 1 ] = Strassen( s[ 1 ], bs[ 1 ][ 1 ], adjN, adjL, adjM );
	M[ 2 ] = Strassen( s[ 2 ], bs[ 0 ][ 0 ], adjN, adjL, adjM );
	M[ 3 ] = Strassen( as[ 1 ][ 1 ], s[ 3 ], adjN, adjL, adjM );
	M[ 4 ] = Strassen( s[ 4 ], s[ 5 ], adjN, adjL, adjM );
	M[ 5 ] = Strassen( s[ 6 ], s[ 7 ], adjN, adjL, adjM );
	M[ 6 ] = Strassen( s[ 8 ], s[ 9 ], adjN, adjL, adjM );

	for ( int i = 0; i < adjN; i++ ) {
		for ( int j = 0; j < adjM; j++ ) {
			c[ i ][ j ] = M[ 4 ][ i ][ j ] + M[ 3 ][ i ][ j ] - M[ 1 ][ i ][ j ] + M[ 5 ][ i ][ j ];
			if ( j + adjM < m ) c[ i ][ j + adjM ] = M[ 0 ][ i ][ j ] + M[ 1 ][ i ][ j ];
			if ( i + adjN < n ) c[ i + adjN ][ j ] = M[ 2 ][ i ][ j ] + M[ 3 ][ i ][ j ];
			if ( i + adjN < n && j + adjM < m ) c[ i + adjN ][ j + adjM ] = M[ 4 ][ i ][ j ] + M[ 0 ][ i ][ j ] - M[ 2 ][ i ][ j ] - M[ 6 ][ i ][ j ];
		}
	}

	del_ab( as, adjN );
	del_ab( bs, adjL );
	del_s( s, adjL, adjN );
	del_M( M, n );

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

	int** ans = Strassen( a, b, n, n, n );
	// for ( int i = -1; i < 10; i++ ) {
	// 	for ( int j = 0; j < 10; j++ ) {
	// 		printf( "%d ", ans[ i ][ j ] );
	// 	}
	// }

	return 0;
}