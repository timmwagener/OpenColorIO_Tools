

#ifndef OCIO_FUNCTIONALITY_H
#define OCIO_FUNCTIONALITY_H

//Ocio_functionality Declaration
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
//OCIO
#include <OpenColorIO/OpenColorIO.h>
//Own


//OCIO Namespace
namespace OCIO = OCIO_NAMESPACE;


//Ocio_functionality
//-----------------------------------------------

namespace OCIO_functionality
{
	
	//OCIO
	OCIO::ConstConfigRcPtr get_config_from_env();
	OCIO::ConstConfigRcPtr get_config_from_file(std::string&);
	std::vector<std::string> get_colorspace_names(OCIO::ConstConfigRcPtr&);
	int get_colorspace_count(OCIO::ConstConfigRcPtr&);
	std::string get_colorspace_name_from_index(OCIO::ConstConfigRcPtr&, int);
	OCIO::ConstProcessorRcPtr get_processor(OCIO::ConstConfigRcPtr&, std::string&, std::string&);
	OCIO::ConstProcessorRcPtr get_processor_from_file_transform(std::string, std::string, int, int);
	void color_transform_single_pixel(float*&, float*&, float*&, OCIO::ConstProcessorRcPtr&);
	std::string get_config_info(OCIO::ConstConfigRcPtr&);
	//Temp
	void print_config_info(OCIO::ConstConfigRcPtr&);
	
}


#endif