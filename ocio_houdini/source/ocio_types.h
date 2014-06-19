

#ifndef OCIO_TYPES_H
#define OCIO_TYPES_H

//Ocio_types Definition
//-----------------------------------------------

/*
Ocio_types
Header that defines custom types (enums and constants) to be used with
ocio houdini.
*/






//include
//-----------------------------------------------
//Std
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
//Boost
#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"
//Houdini
#include <OP/OP_Director.h>
#include <CH/CH_Manager.h>
#include <PRM/PRM_Include.h>
#include <PRM/PRM_Parm.h>
#include <PRM/PRM_SpareData.h>
#include <TIL/TIL_Region.h>
#include <TIL/TIL_Plane.h>
#include <TIL/TIL_Sequence.h>
#include <TIL/TIL_Tile.h>
#include <TIL/TIL_Raster.h>
#include <COP2/COP2_CookAreaInfo.h>
#include <UT/UT_Lock.h>
#include <COP2/COP2_MaskOp.h>
//Own
#include "ocio_functionality.h"



//OCIO Namespace
namespace OCIO = OCIO_NAMESPACE;






//Constants
//-----------------------------------------------
//-----------------------------------------------


namespace ocio_houdini_constants
{
	//LOG_MESSAGES
	extern bool LOG_MESSAGES;
	//INTERNAL_PARMS_VISIBLE
	extern bool INTERNAL_PARMS_VISIBLE;
	//ENABLE_SCOPERGBA
	extern bool ENABLE_SCOPERGBA;

	//OCIOCOLORSPACE_OPERATION_INFO
	extern const char* OCIOCOLORSPACE_OPERATION_INFO;
	//OCIOLOGCONVERT_OPERATION_INFO
	extern const char* OCIOLOGCONVERT_OPERATION_INFO;
	//OCIOFILETRANSFORM_OPERATION_INFO
	extern const char* OCIOFILETRANSFORM_OPERATION_INFO;
	//OCIOCDLTRANSFORM_OPERATION_INFO
	extern const char* OCIOCDLTRANSFORM_OPERATION_INFO;
};







//Types
//-----------------------------------------------
//-----------------------------------------------


//Colorspace_in_or_out
//-----------------------------------------------

enum Colorspace_in_or_out
{
	INPUT_COLORSPACE,
	OUTPUT_COLORSPACE
};




//OCIO_houdini_functionality
//-----------------------------------------------

class OCIO_houdini_functionality
{
public:

	//Constructor / Destructor
	//-----------------------------------------------

	OCIO_houdini_functionality();
	~OCIO_houdini_functionality();
	

	//Color transformation
	//-----------------------------------------------

	//non-static filter called in each static filter
	void filter(COP2_Context& context,
		const TIL_Region* input,
		TIL_Region* output,
		OCIO::ConstProcessorRcPtr& processor);

	//rgb_available
	bool rgb_available(COP2_Context& context,
		const TIL_Region* input,
		TIL_Region* output);

	//filter_ocio
	void filter_ocio(COP2_Context& context,
		const TIL_Region* input,
		TIL_Region* output,
		OCIO::ConstProcessorRcPtr& processor);

	//filter_no_ocio
	void filter_no_ocio(COP2_Context& context,
		const TIL_Region* input,
		TIL_Region* output);

	//Misc
	//-----------------------------------------------
	void log(const char*);
	float get_time();
	bool colors_equal(float*&, float*&, int length = 3);
	

private:

};



#endif