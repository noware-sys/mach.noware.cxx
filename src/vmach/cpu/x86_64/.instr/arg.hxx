#pragma once

class arg
	: virtual public noware::serial
{
	public:
		#include ".arg/.hxx"
		
		arg (void);
		
		// memory reference
		//bool ref;
		//bool var;
		unsigned short int ref;
		
		// number of terms
		// which are accounted for
		unsigned short int terms_nr;
		
		// the terms themselves
		term term1, term2, term3;
		
		
		friend class boost::serialization::access;
		
		template <typename archive>
		void serialize (archive &, unsigned int const &/* version*/);
};