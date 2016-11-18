/***********************************************************************
	Based on map example from Deitel 8-10th Editions

	Extensively modified by Martin Chetlen to illustrate function pointers and function objects

	CS M20
*********************************************************/

// Fig. 22.22: Fig22_22.cpp (8th edition)  (15.18, page 672, in Deitel 9th edition, 15.18, page 689-690, 10th edition)
// Standard Library class map test program.
#include <iostream>
#include <map> // map class-template definition
#include <string>
#include <cstdlib> // for toupper

using namespace std;

class compare
{
public:
	bool operator () ( const string & str1, const string &str2 ) const
	{
		size_t i;
		bool end = false, less = false;
		char c1, c2;

		for (	i = 0;
				!end && i < ( ( str1.length() < str2.length() ) ? str1.length() : str2.length() );
				++i )
		{
			c1= toupper( str1[ i ] );
			c2= toupper( str2[ i ] );
			if ( c1 < c2 )
				end = less = true;
			else if ( c1 > c2 )
				end = true;
		}

		if ( !end && str1.length() < str2.length() )
			less = true;

		return less;
	}
};

bool functionPointer( const string & str1, const string &str2 )
{
	size_t i;
	bool end = false, less = false;
	char c1, c2;

	for (	i = 0;
			!end && i < ( ( str1.length() < str2.length() ) ? str1.length() : str2.length() );
			++i )
	{
		c1= toupper( str1[ i ] );
		c2= toupper( str2[ i ] );
		if ( c1 < c2 )
			end = less = true;
		else if ( c1 > c2 )
			end = true;
	}

	if ( !end && str1.length() < str2.length() )
		less = true;

	return less;
}

// define short name for map type used in this program
typedef map< int, double, less< int > > Mid;




int main()
{
   Mid pairs;

   // insert eight value_type objects in pairs
   pairs.insert( make_pair( 15, 2.7 ) );  // Can use make_pair or value_type
   pairs.insert( Mid::value_type( 30, 111.11 ) );
   pairs.insert( Mid::value_type( 5, 1010.1 ) );
   pairs.insert( make_pair( 10, 22.22 ) );
   pairs.insert( Mid::value_type( 25, 33.333 ) );
   pairs.insert( Mid::value_type( 5, 77.54 ) ); // dup ignored
   pairs.insert( Mid::value_type( 20, 9.345 ) );
   pairs.insert( make_pair( 15, 99.3 ) ); // dup ignored

   cout << "pairs contains:\nKey\tValue\n";
   
   // use const_iterator to walk through elements of pairs
   for ( Mid::const_iterator iter = pairs.begin();        
      iter != pairs.end(); ++iter )                       
      cout << iter->first << '\t' << iter->second << '\n';

   pairs[ 25 ] = 9999.99; // use subscripting to change value for key 25
   pairs[ 40 ] = 8765.43; // use subscripting to insert value for key 40
   
   cout << "\nAfter subscript operations, pairs contains:\nKey\tValue\n";
   
   // use const_iterator to walk through elements of pairs
   for ( Mid::const_iterator iter2 = pairs.begin();
      iter2 != pairs.end(); ++iter2 )           
      cout << iter2->first << '\t' << iter2->second << '\n';




   typedef map< string, string, less< string > > sMap;

   cout << "\n\nCreate new map using strings" << endl;

   sMap strPair;

   strPair.insert( make_pair( "Elmer Bernstein", "To Kill a Mockingbird" ) );
   strPair.insert( make_pair( "Bernard Herrmann", "The Ghost and Mrs. Muir" ) );
   strPair.insert( make_pair( "Alex North", "Cleopatra" ) );
   strPair.insert( make_pair( "Alfred Newman", "How The West Was Won" ) );	
   strPair.insert( make_pair( "Bronislau Kaper", "Mutiny on the Bounty" ) );
   strPair.insert( make_pair( "Franz Waxman", "Sunset Boulevard" ) );
   strPair.insert( make_pair( "Victor Young", "Around the World in 80 Days" ) );
   strPair.insert( make_pair( "elmer Bernstein", "The Great Escape" ) );
   strPair.insert( make_pair( "elmer bernstein", "Hawaii" ) );

   // use const_iterator to walk through elements of strPair
   for ( sMap::const_iterator iter3 = strPair.begin();
      iter3 != strPair.end(); ++iter3 )           
      cout << iter3->first << '\t' << iter3->second << '\n';

   cout << endl;

   cout << "\nChange some of the values" << endl;
   strPair[ "Elmer Bernstein" ] = "The Magnificent Seven";
   strPair[ "Bernard Herrmann" ] = "The 7th Voyage of Sinbad";

   // use const_iterator to walk through elements of strPair
   for ( sMap::const_iterator iter3 = strPair.begin();
      iter3 != strPair.end(); ++iter3 )           
      cout << iter3->first << '\t' << iter3->second << '\n';
                          
   cout << endl;



   typedef map< string, string, compare > scMap;

   cout << "\n\nNew map with function OBJECT to perform comparison" << endl;
   scMap strPair2;

   strPair2.insert( make_pair( "Elmer Bernstein", "To Kill a Mockingbird" ) );
   strPair2.insert( make_pair( "Bernard Herrmann", "The Ghost and Mrs. Muir" ) );
   strPair2.insert( make_pair( "Alex North", "Cleopatra" ) );
   strPair2.insert( make_pair( "Alfred Newman", "How The West Was Won" ) );	
   strPair2.insert( make_pair( "Bronislau Kaper", "Mutiny on the Bounty" ) );
   strPair2.insert( make_pair( "Franz Waxman", "Sunset Boulevard" ) );
   strPair2.insert( make_pair( "Victor Young", "Around the World in 80 Days" ) );
   strPair2.insert( make_pair( "elmer Bernstein", "The Great Escape" ) );
   strPair2.insert( make_pair( "elmer bernstein", "Hawaii" ) );

   // use const_iterator to walk through elements of strPair2
   for ( scMap::const_iterator iter3 = strPair2.begin();
      iter3 != strPair2.end(); ++iter3 )           
      cout << iter3->first << '\t' << iter3->second << '\n';

   cout << endl;

   cout << "\nChange some of the values" << endl;
   strPair2[ "Elmer Bernstein" ] = "The Magnificent Seven";
   strPair2[ "Bernard Herrmann" ] = "The 7th Voyage of Sinbad";

   // use const_iterator to walk through elements of strPair2
   for ( scMap::const_iterator iter3 = strPair2.begin();
      iter3 != strPair2.end(); ++iter3 )           
      cout << iter3->first << '\t' << iter3->second << '\n';
                          
   cout << endl;


typedef bool (*fp) ( const string &, const string & );

typedef map<string, string, fp> scfpMap;

   cout << "\n\nNew map with function POINTER to perform comparison" << endl;
   scfpMap strPair3( functionPointer );

   strPair3.insert( make_pair( "Elmer Bernstein", "To Kill a Mockingbird" ) );
   strPair3.insert( make_pair( "Bernard Herrmann", "The Ghost and Mrs. Muir" ) );
   strPair3.insert( make_pair( "Alex North", "Cleopatra" ) );
   strPair3.insert( make_pair( "Alfred Newman", "How The West Was Won" ) );	
   strPair3.insert( make_pair( "Bronislau Kaper", "Mutiny on the Bounty" ) );
   strPair3.insert( make_pair( "Franz Waxman", "Sunset Boulevard" ) );
   strPair3.insert( make_pair( "Victor Young", "Around the World in 80 Days" ) );
   strPair3.insert( make_pair( "elmer Bernstein", "The Great Escape" ) );
   strPair3.insert( make_pair( "elmer bernstein", "Hawaii" ) );

   // use const_iterator to walk through elements of strPair3
   for ( scfpMap::const_iterator iter3 = strPair3.begin();
      iter3 != strPair3.end(); ++iter3 )           
      cout << iter3->first << '\t' << iter3->second << '\n';

   cout << endl;

   cout << "\nChange some of the values" << endl;
   strPair3[ "Elmer Bernstein" ] = "The Magnificent Seven";
   strPair3[ "Bernard Herrmann" ] = "The 7th Voyage of Sinbad";

   // use const_iterator to walk through elements of strPair3
   for ( scfpMap::const_iterator iter3 = strPair3.begin();
      iter3 != strPair3.end(); ++iter3 )           
      cout << iter3->first << '\t' << iter3->second << '\n';
                          
   cout << endl;

} // end main



