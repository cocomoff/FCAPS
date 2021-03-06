// Initial software, Aleksey Buzmakov, Copyright (c) INRIA and University of Lorraine, GPL v2 license, 2011-2015, v0.7

// Author: Aleksey Buzmakov
// Description: Implementation of AddIntent algorithm.

#ifndef ADDINTENTLATTICEBUILDER_H_
#define ADDINTENTLATTICEBUILDER_H_
///////////////////////////////////////////////////////////////////////////////////
#include "LatticeBuilder.h"
#include "Lattice.h"

#include <ListWrapper.h>
#include <fcaps/CompareResults.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef CList<TLatticeNodeId> CLatticeNodesList;
// The element of our stack to avoid recursion.
struct CAddIntentState {
	// Intent for processing on the stage
	DWORD Intent;
	// The maximal node which give birth to the Intent
	TLatticeNodeId MaxGeneratorConcept;
	// The set of nodes to be set as new parents
	CLatticeNodesList ParentCandidates;
	// The set of parrents
	CLatticeNodesList NewParents;
};

///////////////////////////////////////////////////////////////////////////////////

// Not recursive implementation of AddIntent algorithm.

// Merwe D., Obiedkov S., Kourie D.
// AddIntent: A New Incremental Algorithm for Constructing Concept Lattices
// // Concept Lattices / ed. Eklund P. Berlin, Heidelberg: Springer Berlin Heidelberg, 2004. Vol. 2961. P. 372–385.
class CAddIntentLatticeBuilder : public ILatticeBuilder {
public:
	CAddIntentLatticeBuilder();

	// Methods of ILatticeBuilder
	virtual void Initialize(
		const CSharedPtr<IExtentStorage>& extStorage,
		const CSharedPtr<IIntentStorage>& intentStorage );
	virtual void SetResultLattice( CLattice& result );
	virtual void SetPrunner( const CSharedPtr<IPatternPrunner>& prunner );
	virtual void AddObject( DWORD objectNum, DWORD patternID );
	virtual void ProcessAllObjectsAddition()
		{}

protected:
	// Find the biggest concept giving the birth to the intent, i.e. more specific than the intent.
	//  (The function GetMaximalConcept from original AddIntent algorithm).
	//  intent -- the intent we are looking for
	//  generatorConcept -- the start concept for the search
	//  maxGeneratorConcept -- the result concept
	//  result: value if the intent of maxGeneratorConcept is equal to 'intent'
	virtual bool FindMaxConcept(
		DWORD intent, TLatticeNodeId generatorConcept,
		TLatticeNodeId& maxGeneratorConcept );
	// Find the intent generated by a parent of max concept generator for the given intent
	//  returns the result intent or zero if the parent is trivial, i.e. intent == parentCandidate.Intent.
	virtual DWORD FindGeneratedIntentByParent(
		DWORD intent, const TLatticeNodeId& parentCandidate );
	// Initialization of a new concept.
	virtual void InitializeNewConcept(
		DWORD extent, DWORD intent,
		const TLatticeNodeId& node );

	// The object to process extents.
	CSharedPtr<IExtentStorage> extStorage;
	// The object to compare patterns.
	CSharedPtr<IIntentStorage> comparator;
	// The object to prune patterns.
	CSharedPtr<IPatternPrunner> prunner;
	// Result will be here.
	CLattice* lattice;
	// The currently processing object.
	DWORD processingObject;

	CLatticeConcept& getConcept( TLatticeNodeId id );
	CLatticeNode& getNode( TLatticeNodeId id );

private:
	// The original algorithm is recursive. Here we don't use stack. Instead we use this.
	CList<CAddIntentState> statesForProcess;
	// The less general concept with desciption.
	TLatticeNodeId lessConcept;

	// Description of currently processing object
	DWORD currObjDescrID;

	void addIntent( DWORD descriptionID );
	void addState( DWORD intent, TLatticeNodeId generatorConcept );
	void addConcept( TLatticeNodeId resultConcept );
	void processStates();
	void addConceptToLattice();
	void addNewObjectToConceptTree( DWORD newObjectDescription, TLatticeNodeId concept );
	void filterLattice();
};

////////////////////////////////////////////////////////////////////

// A version, where FindMaxConcept is performed by calculating and preserving intersections.
//  Uses field UserData of lattice.
class CAddIntentLatticeBuilderExt : public CAddIntentLatticeBuilder {
public:
	CAddIntentLatticeBuilderExt()
		{}

	// Methods of CAddIntentLatticeBuilderExt
	virtual void ProcessAllObjectsAddition();

protected:
	// Methods of CAddIntentLatticeBuilder
	virtual bool FindMaxConcept(
		DWORD intent, TLatticeNodeId generatorConcept,
		TLatticeNodeId& maxGeneratorConcept );
	virtual DWORD FindGeneratedIntentByParent(
		DWORD intent, const TLatticeNodeId& parentCandidate );
	virtual void InitializeNewConcept(
		DWORD extent, DWORD intent,
		const TLatticeNodeId& node );

private:
	TCompareResult isIntentMoreEqGeneral( DWORD intent, const TLatticeNodeId& parentCandidate );
	TCompareResult isIntentMoreEqGeneralSmpl( DWORD intent, const TLatticeNodeId& parentCandidate );
	TCompareResult isIntentMoreEqGeneralExt( DWORD intent, const TLatticeNodeId& parentCandidate );
};

///////////////////////////////////////////////////////////////////////////////////
#endif // ADDINTENTLATTICEBUILDER_H_
