

//Ocio_types Definition
//-----------------------------------------------

/*
Ocio_types
Definitions for OCIO_Houdini wide constants and types.
*/






//include
//-----------------------------------------------

//Own
#include "ocio_types.h"





//Constants
//-----------------------------------------------
//-----------------------------------------------


namespace ocio_houdini_constants
{
	//LOG_MESSAGES
	bool LOG_MESSAGES = false;
	//INTERNAL_PARMS_VISIBLE
	bool INTERNAL_PARMS_VISIBLE = false;

	//OCIOCOLORSPACE_OPERATION_INFO
	const char* OCIOCOLORSPACE_OPERATION_INFO = "This operator loads an ocio config file either from\
												\nan $OCIO env. variable or a custom path.\
												\nYou can then apply all LUTs and transformations\
												\nthat are described in that config. If no config is\
												\ngiven, the operation will result in a noOp.";
	//OCIOLOGCONVERT_OPERATION_INFO
	const char* OCIOLOGCONVERT_OPERATION_INFO = "This operator converts from linear to log or log to linear.";
	//OCIOFILETRANSFORM_OPERATION_INFO
	const char* OCIOFILETRANSFORM_OPERATION_INFO = "This operator loads a LUT file and applies its transformation.";
	//OCIOCDLTRANSFORM_OPERATION_INFO
	const char* OCIOCDLTRANSFORM_OPERATION_INFO = "This operator grades the incoming pixels according\
												  \nto the ACES standard. Alternatively you can load\
												  \na .cc grade file or export such a file from your\
												  \ncurrent SOP (slope, offset, power) settings.";
};







//Types
//-----------------------------------------------
//-----------------------------------------------



