

#ifndef OCIO_EXPORT_CC
#define OCIO_EXPORT_CC



//Ocio_export_cc Declaration
//-----------------------------------------------


//Constants
//-----------------------------------------------



//include
//-----------------------------------------------
//Std
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
//Boost
#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"
//Own
#include "ocio_functionality.h"
//Maya
#include <maya/MArgList.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MFileIO.h>
#include <maya/MFloatVector.h>
#include <maya/MSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MFnDependencyNode.h>





//Ocio_export_cc
//-----------------------------------------------
class Ocio_export_cc : public MPxCommand
{
public:
	Ocio_export_cc();
	virtual MStatus doIt(const MArgList&);
	static void* creator();
	static MSyntax new_syntax();

	//Own
	void get_cc_parameters(std::string, MFloatVector&, MFloatVector&, MFloatVector&, float&, int&);
	void get_sop_as_float_ptr(float*&,
								MFloatVector&,
								MFloatVector&,
								MFloatVector&);
	std::string get_cc_xml(float*&, float&, int&);
	void write_to_file(std::string&, std::string&);
	MPlug get_plug(std::string, std::string);
	std::string split_filename(const std::string&);

private:
	//flag_names
	static const char* flag_source_node;
	static const char* flag_long_source_node;

	static const char* flag_cc_path;
	static const char* flag_long_cc_path;
};



#endif