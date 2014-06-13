

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







//Constants
//-----------------------------------------------
//-----------------------------------------------


namespace ocio_houdini_constants
{
	//LOG_MESSAGES
	extern bool LOG_MESSAGES;
	//INTERNAL_PARMS_VISIBLE
	extern bool INTERNAL_PARMS_VISIBLE;

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









#endif