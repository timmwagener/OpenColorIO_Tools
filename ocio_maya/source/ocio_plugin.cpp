

//ocio_plugin Implementation
//-----------------------------------------------
/*
Exported functions that maya uses to register the plugin 
*/

//Preprocessor
//-----------------------------------------------
#define EXPORT __declspec(dllexport)


//include
//-----------------------------------------------
//ocio_maya
#include "ocio_test.h"
#include "ocio_simple_transform.h"
#include "ocio_colorspace.h"
//Maya
#include <maya/MFnPlugin.h>





//initialize
//-----------------------------------------------
EXPORT MStatus initializePlugin(MObject obj)
{
	MStatus status;

	//initialize plugin functionset
	MFnPlugin fnPlugin(obj, "Timm Wagener", "0.1", "2012");

	//ocio_test
	status = fnPlugin.registerNode(Ocio_test::type_name, Ocio_test::id, Ocio_test::create, Ocio_test::initialize, MPxNode::kDependNode);
	//ocio_simple_transform
	status = fnPlugin.registerNode(Ocio_simple_transform::type_name, Ocio_simple_transform::id, Ocio_simple_transform::create, Ocio_simple_transform::initialize, MPxNode::kDependNode);
	//OCIOColorspace
	status = fnPlugin.registerNode(Ocio_colorspace::type_name, Ocio_colorspace::id, Ocio_colorspace::create, Ocio_colorspace::initialize, MPxNode::kDependNode);

	return MStatus::kSuccess;
}


//uninitialize
//-----------------------------------------------
EXPORT MStatus uninitializePlugin(MObject obj)
{
	MStatus status;

	//initialize plugin functionset
	MFnPlugin fnPlugin(obj);

	//ocio_test
	status = fnPlugin.deregisterNode(Ocio_test::id);
	//ocio_simple_transform
	status = fnPlugin.deregisterNode(Ocio_simple_transform::id);
	//OCIOColorspace
	status = fnPlugin.deregisterNode(Ocio_colorspace::id);
	

	return MStatus::kSuccess;
}
