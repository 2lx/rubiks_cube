#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

void writeMatrix( GLfloat * Matrix, const int rowC, const int colC )
{
	std::cout.precision( 4 );

	for ( int i = 0; i < rowC * colC; i++ )
	{
		std::cout.width( 8 );
		std::cout << std::fixed << std::right << Matrix[ i ] << " ";

		if ( ( i % colC ) == ( colC - 1 ) )
			std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout.flush();
}

void writeVector3D( const Vector3D vec )
{
	std::cout.precision( 1 );

	std::cout.width( 6 );
	std::cout << std::fixed << std::right << vec.x() << " ";
	std::cout.width( 6 );
	std::cout << std::fixed << std::right << vec.y() << " ";
	std::cout.width( 6 );
	std::cout << std::fixed << std::right << vec.z() << " ";
//	std::cout << std::endl;

	std::cout.flush();
}

#endif // OUTPUT_H_INCLUDED
