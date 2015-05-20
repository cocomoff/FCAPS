#include "StabilityCalculation.h"

#include <fcaps/modules/VectorBinarySetDescriptor.h>

// Increase size of the table if neccesary, to include at least minimalSize elements.
void expandCollection( CSharedPtr<CVectorBinarySetJoinComparator>& cmp,
	DWORD minimalSize, CBinarySetCollection& table )
{
	for( size_t i = table.size(); i <= minimalSize; ++i ) {
		table.push_back( CSharedPtr<CVectorBinarySetDescriptor>(
			cmp->NewPattern(), CPatternDeleter(cmp) ) );
	}

	assert( table.size() > minimalSize );
}

void AddColumnToCollection( CSharedPtr<CVectorBinarySetJoinComparator>& cmp,
	DWORD columnNum, const CList<DWORD>& values, CBinarySetCollection& table )
{
	CList<DWORD>::CConstReverseIterator i = values.RBegin();
	for( ; i!= values.REnd(); ++i ) {
		expandCollection( cmp, *i, table );
		assert( table.size() > *i );
		cmp->AddValue( columnNum, *table[*i] );
	}
}
