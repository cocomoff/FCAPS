// Initial software, Aleksey Buzmakov, Copyright (c) INRIA and University of Lorraine, GPL v2 license, 2011-2015, v0.7

#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include <common.h>

#include <string>
#include <vector>

const char ContextFilterModuleType[] = "ContextFilters";
const char LatticeFilterModuleType[] = "LatticeFilters";

interface IFilter : public virtual IObject {
	// Returns the list of the resulting files.
	virtual const std::vector<std::string>& GetResultFiles() const = 0;
	// Get/Set input file
	virtual const std::string& GetInputFile() const = 0;
	virtual void SetInputFile( const std::string& ) = 0;
	// Process the input file
	virtual void Process() = 0;
};

#endif // FILTER_H_INCLUDED
