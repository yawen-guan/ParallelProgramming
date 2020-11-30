#include "../include/constant.hpp"
#include "boost/shared_array.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

typedef boost::shared_array< boost::shared_array< int > > SA;

SA makeSA( int n ) {
	SA ptr( new boost::shared_array< int >[ n ] );
	for ( int i = 0; i < n; i++ )
		ptr[ i ].reset( new int[ n ] );
	return ptr;
}

void readin( SA a, SA b, int n ) {
	FILE* f = fopen( "../data/matrix", "r" );

	for ( int i = 0; i < n; i++ )
		for ( int j = 0; j < n; j++ )
			fscanf( f, "%d", &a[ i ][ j ] );
	for ( int i = 0; i < n; i++ )
		for ( int j = 0; j < n; j++ )
			fscanf( f, "%d", &b[ i ][ j ] );

	fclose( f );
}

void divide4( SA u, SA uu[ 2 ][ 2 ], int n ) {
	for ( int i = 0; i < 2; i++ )
		for ( int j = 0; j < 2; j++ )
			uu[ i ][ j ] = makeSA( n / 2 );
	int mid = ( n - 1 ) / 2;
	for ( int i = 0; i <= mid; i++ )
		for ( int j = 0; j <= mid; j++ )
			uu[ 0 ][ 0 ][ i ][ j ] = u[ i ][ j ];
	for ( int i = 0; i <= mid; i++ )
		for ( int j = mid + 1; j < n; j++ )
			uu[ 0 ][ 1 ][ i ][ j - mid - 1 ] = u[ i ][ j ];
	for ( int i = mid + 1; i < n; i++ )
		for ( int j = 0; j <= mid; j++ )
			uu[ 1 ][ 0 ][ i - mid - 1 ][ j ] = u[ i ][ j ];
	for ( int i = mid + 1; i < n; i++ )
		for ( int j = mid + 1; j < n; j++ )
			uu[ 1 ][ 1 ][ i - mid - 1 ][ j - mid - 1 ] = u[ i ][ j ];
}

void addup( SA res, SA C00, SA C01, SA C10, SA C11, int n ) {
	int mid = ( n - 1 ) / 2;
	for ( int i = 0; i <= mid; i++ )
		for ( int j = 0; j <= mid; j++ )
			res[ i ][ j ] = C00[ i ][ j ];
	for ( int i = 0; i <= mid; i++ )
		for ( int j = mid + 1; j < n; j++ )
			res[ i ][ j ] = C01[ i ][ j - mid - 1 ];
	for ( int i = mid + 1; i < n; i++ )
		for ( int j = 0; j <= mid; j++ )
			res[ i ][ j ] = C10[ i - mid - 1 ][ j ];
	for ( int i = mid + 1; i < n; i++ )
		for ( int j = mid + 1; j < n; j++ )
			res[ i ][ j ] = C11[ i - mid - 1 ][ j - mid - 1 ];
}

SA add( SA u, SA v, int n ) {
	SA sum = makeSA( n );
	for ( int i = 0; i < n; i++ )
		for ( int j = 0; j < n; j++ )
			sum[ i ][ j ] = u[ i ][ j ] + v[ i ][ j ];
	return sum;
}

SA reduce( SA u, SA v, int n ) {
	SA sum = makeSA( n );
	for ( int i = 0; i < n; i++ )
		for ( int j = 0; j < n; j++ )
			sum[ i ][ j ] = u[ i ][ j ] - v[ i ][ j ];
	return sum;
}

SA mult( SA u, SA v, int n ) {
	SA res = makeSA( n );
	if ( n == 2 ) {
		for ( int i = 0; i < 2; i++ )
			for ( int j = 0; j < 2; j++ ) {
				res[ i ][ j ] = 0;
				for ( int k = 0; k < 2; k++ )
					res[ i ][ j ] += u[ i ][ k ] * v[ k ][ j ];
			}
	}
	else {
		SA uu[ 2 ][ 2 ], vv[ 2 ][ 2 ];
		divide4( u, uu, n );
		divide4( v, vv, n );
		SA M1 = mult( add( uu[ 0 ][ 0 ], uu[ 1 ][ 1 ], n / 2 ), add( vv[ 0 ][ 0 ], vv[ 1 ][ 1 ], n / 2 ), n / 2 );
		SA M2 = mult( add( uu[ 1 ][ 0 ], uu[ 1 ][ 1 ], n / 2 ), vv[ 0 ][ 0 ], n / 2 );
		SA M3 = mult( uu[ 0 ][ 0 ], reduce( vv[ 0 ][ 1 ], vv[ 1 ][ 1 ], n / 2 ), n / 2 );
		SA M4 = mult( uu[ 1 ][ 1 ], reduce( vv[ 1 ][ 0 ], vv[ 0 ][ 0 ], n / 2 ), n / 2 );
		SA M5 = mult( add( uu[ 0 ][ 0 ], uu[ 0 ][ 1 ], n / 2 ), vv[ 1 ][ 1 ], n / 2 );
		SA M6 = mult( reduce( uu[ 1 ][ 0 ], uu[ 0 ][ 0 ], n / 2 ), add( vv[ 0 ][ 0 ], vv[ 0 ][ 1 ], n / 2 ), n / 2 );
		SA M7 = mult( reduce( uu[ 0 ][ 1 ], uu[ 1 ][ 1 ], n / 2 ), add( vv[ 1 ][ 0 ], vv[ 1 ][ 1 ], n / 2 ), n / 2 );

		SA C00 = add( reduce( add( M1, M4, n / 2 ), M5, n / 2 ), M7, n / 2 );
		SA C01 = add( M3, M5, n / 2 );
		SA C10 = add( M2, M4, n / 2 );
		SA C11 = add( add( reduce( M1, M2, n / 2 ), M3, n / 2 ), M6, n / 2 );
		addup( res, C00, C01, C10, C11, n );
	}
	return res;
}

int main() {
	int n = N;
	SA a = makeSA( n );
	SA b = makeSA( n );
	readin( a, b, n );
	auto ans = mult( a, b, n );
	// for ( int i = 0; i < n; i++ )
	// for ( int j = 0; j < n; j++ )
	// printf( "%d ", ans[ i ][ j ] );
	return 0;
}