// Initial software, Aleksey Buzmakov, Copyright (c) Higher School of Economics, Russia, GPL v2 license, 2011-2016, v0.7

// Author: Aleksey Buzmakov
// Description: Implementation of pattern enumeration interface. It is realy on the original code of Gaston from an external library.
//  In order to rely on existing code of Gaston, the most simple thing is to find places in this code where patterns are reported
//  and then report the patterns by a callback. However in this case pattern reported by callback is hard to translate to the IPatternEnumerator interface
//  Accordingly, we run Gaston in parallel thread and are switching between threads when we want to get next pattern by the callback.

#ifndef GASTONGRAPHPATTERNENUMERATOR_H
#define GASTONGRAPHPATTERNENUMERATOR_H

#include <fcaps/PatternEnumerator.h>

#include <Library.h>
#include <LibGastonForSofia.h>

#include <fcaps/Module.h>
#include <fcaps/ModuleTools.h>

#include <boost/unordered_map.hpp>
#include <vector>

////////////////////////////////////////////////////////////////////////

const char GastonGraphPatternEnumeratorModule [] = "GastonGraphPatternEnumeratorModule";

////////////////////////////////////////////////////////////////////////

class CGastonGraphPatternEnumerator : public IPatternEnumerator, public IModule {
public:
    CGastonGraphPatternEnumerator();
    ~CGastonGraphPatternEnumerator();

	// Methods of IPatternEnumerator
	virtual void AddObject( DWORD objectNum, const JSON& intent );
    virtual bool GetNextPattern( TCurrentPatternUsage usage, CPatternImage& pattern );
    virtual void ClearMemory( CPatternImage& pattern );

	// Methods of IModule
	virtual void LoadParams( const JSON& );
	virtual JSON SaveParams() const;
	virtual const char* const GetType() const
	{ return PatternEnumeratorModuleType; }
	virtual const char* const GetName() const
	{ return GastonGraphPatternEnumeratorModule; }

	// A special function to run a new thread.
	void RunGastonThread();
private:
	class CLabelMap {
	public:
		// Returns label by label id
		const std::string& GetLabel( DWORD id ) const;
		// Returns id by label
		const DWORD GetId( const std::string& label );

	private:
		typedef boost::unordered_map<std::string, DWORD> CLabelToIdMap;
	private:
		CLabelToIdMap labelToId;
		std::vector<std::string> idToLabel;
	};
	struct CSyncData;
private:
	static CModuleRegistrar<CGastonGraphPatternEnumerator> registar;

	// Path to library with Gaston algorithm
	std::string libraryPath;
	// Library with Gaston algo.
	Library gastonLib;
	// Path to file to write the input graphs to
	std::string inputPath;
	// Should the input be removed
	bool removeInput;
	// To write the input file
	CDestStream inputStream;
	// Path for saving the patterns
	std::string patternPath;
	CDestStream patternStream;
	// Original object numbers
	std::vector<DWORD> origObjIDs;
	// Label maps for vertices and edges.
	CLabelMap vertexLabelMap;
	CLabelMap edgeLabelMap;
	// Function to run GastonAlgo
	RunGastonFunc runGaston;
	// All data for syncronization
	CPtrOwner<CSyncData> syncData;
	// A flag to mark the fact the gaston is started.
	bool isGastonRun;
	// Min support to be passed to Gaston
	int gastonMinSupport;
	// Max size of a pattern to be passed to Gaston
	int gastonMaxPtrnSize;
	// Running mode for Gaston
	TGastonRunningMode gastonMode;

	static bool gastonCallback( LibGastonDataRef data, const LibGastonGraph* graph );

	void loadLibrary();
	void createGastonThread();
	bool registerGraph( const LibGastonGraph* graph );
    bool getNextPattern( TCurrentPatternUsage usage, CPatternImage& pattern );

	void writePattern( const LibGastonGraph* graph );
};

////////////////////////////////////////////////////////////////////////

#endif // GASTONGRAPHPATTERNENUMERATOR_H
