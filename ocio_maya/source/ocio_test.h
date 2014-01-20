

#ifndef OCIO_TEST_H
#define OCIO_TEST_H

//Ocio_test Declaration
//-----------------------------------------------


//Constants
//-----------------------------------------------



//include
//-----------------------------------------------
//Std
#include <string>
#include <iostream>
#include <vector>
//Boost
#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/Signals2.hpp"
//Own
#include "ocio_functionality.h"
//Maya
#include <maya/MPxNode.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MDataBlock.h>
#include <maya/MString.h>
#include <maya/MDGModifier.h>
#include <maya/MStatus.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MFloatVector.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MDataHandle.h>
#include <maya/MFnStringData.h>


//Boost_signal_helloworld
//-----------------------------------------------
struct Boost_signal_helloworld
{
	//executed when signal is emitted
	void operator()() const
	{
		std::cout << "Hello, World from Postconstructor!" << std::endl;
	}
};



//Ocio_test
//-----------------------------------------------
class Ocio_test: public MPxNode
{
public:
	//Attributes
	static MTypeId id;
	static MString type_name;

	static MObject a_verbose;
	
	static MObject a_change_list;
	static MObject a_last_change_index;

	static MObject a_input_color;
	static MObject a_width;
	static MObject a_height;

	static MObject a_config_path;

	static MObject a_output_color;

	

	//Methods
	//Maya
	Ocio_test();
	virtual ~Ocio_test();
	static void* create();
	static MStatus initialize();
	virtual void postConstructor();
	virtual MStatus setDependentsDirty(const MPlug&, MPlugArray&);
	virtual MStatus compute(const MPlug &plug, MDataBlock &data);
	//Own
	void rebuild_input_colorspace(int&, MDataBlock&);
	void create_enum_attribute(std::string, std::vector<std::string>&, MObject&);
	void remove_attribute(MObject&, std::string);
	void update_enum_attribute(std::string, std::vector<std::string>&, MObject&);
	bool attr_connected(std::string);
	bool all_attrs_connected();
	void set_output_color(MFloatVector&, MDataBlock&);
	//Temp
	void print_config_info();


private:
	//Attributes
	boost::signals2::signal<void ()> sig_postconstructor;
	Boost_signal_helloworld* boost_signal_helloworld;

	//Methods


};
#endif