#include "../include/constant.hpp"
#include <cstdio>
#include <iostream>
#include <random>

using namespace std;

const int MAX = ( int )1e4;

void randomNumberGenerate( FILE* fout ) {
	random_device rd;
	mt19937 gen( rd() );
	uniform_int_distribution<> dis( -MAX, MAX );

	for ( int t = 0; t < 2; t++ ) {
		for ( int i = 0; i < N; i++ )
			for ( int j = 0; j < N; j++ )
				fprintf( fout, "%d ", ( int )dis( gen ) );
		fprintf( fout, "%c%c%c", '\n', '\n', '\n' );
	}
}

int main() {
	FILE* f = fopen( "../data/matrix", "w" );
	randomNumberGenerate( f );
	fclose( f );
	return 0;
}