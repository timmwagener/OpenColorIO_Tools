

//Ocio_functionality Definitions
//-----------------------------------------------
//-----------------------------------------------







//Constants
//-----------------------------------------------
//-----------------------------------------------







//include
//-----------------------------------------------
//-----------------------------------------------

//Own
#include "ocio_functionality.h"










//Ocio
//-----------------------------------------------
//-----------------------------------------------

//get_config_from_env
OCIO::ConstConfigRcPtr OCIO_functionality::get_config_from_env()
{
	//clear all cachces
	OCIO::ClearAllCaches();
	
	//config
	OCIO::ConstConfigRcPtr config;
	
	try
	{
		//assign config
		config = OCIO::Config::CreateFromEnv();

		//sanity check (throws exception if something is wrong)
		config->sanityCheck();
	}
	catch (OCIO::Exception& exception)
	{
		std::cerr << "OpenColorIO Error: " << exception.what() << "\nReturning empty config." << std::endl;
		return 0;
	}
	
	return config;
};

//get_config_from_file
OCIO::ConstConfigRcPtr OCIO_functionality::get_config_from_file(std::string& file_path)
{
	//clear all cachces
	OCIO::ClearAllCaches();
	
	//config
	OCIO::ConstConfigRcPtr config;
	
	try
	{
		//assign config
		config = OCIO::Config::CreateFromFile(file_path.c_str());
		
		//sanity check (throws exception if something is wrong)
		config->sanityCheck();
	}
	catch (OCIO::Exception& exception)
	{
		std::cerr << "OpenColorIO Error: " << exception.what() << "\nReturning empty config." << std::endl;
		return 0;
	}

	return config;
};

//get_colorspace_names
std::vector<std::string> OCIO_functionality::get_colorspace_names(OCIO::ConstConfigRcPtr& config)
{
	//colorspace_names
	std::vector<std::string> colorspace_names;

	//config exists
	if (!config)
	{
		//log
		std::cout << "Config does not exists. Returning empty colorspace names vector." << std::endl;
		//no_config entry
		colorspace_names.push_back(std::string("no_config"));
		return colorspace_names;
	};

	
	//colorspace_count
	int colorspace_count = config->getNumColorSpaces();

	
	//iterate and set values
	for (int index = 0; index < colorspace_count; index++)
	{
		//append to vector
		std::string colorspace_name(config->getColorSpaceNameByIndex(index));
		colorspace_names.push_back(colorspace_name);
	};

	return colorspace_names;

};

//get_colorspace_count
int OCIO_functionality::get_colorspace_count(OCIO::ConstConfigRcPtr& config)
{
	
	//config exists
	if (!config)
	{
		//log
		std::cout << "Config does not exists. Returning zero." << std::endl;
		return 0;
	};

	//return count
	return config->getNumColorSpaces();

};

//get_colorspace_name_from_index
std::string OCIO_functionality::get_colorspace_name_from_index(OCIO::ConstConfigRcPtr& config, int index)
{
	//colorspace_name
	std::string colorspace_name(config->getColorSpaceNameByIndex(index));

	return colorspace_name;
};

//get_processor
OCIO::ConstProcessorRcPtr OCIO_functionality::get_processor(OCIO::ConstConfigRcPtr& config,
	std::string& input_colorspace,
	std::string& output_colorspace)
{
	//processor
	OCIO::ConstProcessorRcPtr processor = config->getProcessor(input_colorspace.c_str(),
		output_colorspace.c_str());
	//processor exists
	if (!processor)
	{
		//log
		std::cout << "Processor does not exists" << std::endl;
	};

	return processor;
};

//color_transform_single_pixel
void OCIO_functionality::color_transform_single_pixel(float*& r, float*& g, float*& b, OCIO::ConstProcessorRcPtr& processor)
{
	//planar_image
	OCIO::PlanarImageDesc planar_image(r, g, b, NULL, 1, 1);
	//apply transform
	processor->apply(planar_image);
};

//get_config_info
std::string OCIO_functionality::get_config_info(OCIO::ConstConfigRcPtr& config)
{
	//config_info
	std::string config_info("No Config");

	//config exists
	if (!config)
	{
		//log
		std::cout << "Config does not exists. No config info aquired" << std::endl;
		//return
		return config_info;
	};

	
	
	//ColorSpace Names
	//-----------------------------------------------
	std::vector<std::string> vec_colorspace_names;
	for (int index = 0; index < config->getNumColorSpaces(); index++)
	{
		//append to vector
		std::string colorspace_name(config->getColorSpaceNameByIndex(index));
		vec_colorspace_names.push_back(colorspace_name);
	};

	//oss_colorspace_names
	std::ostringstream oss_colorspace_names;
	if (!vec_colorspace_names.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(vec_colorspace_names.begin(), vec_colorspace_names.end() - 1,
			std::ostream_iterator<std::string>(oss_colorspace_names, ", "));

		// Now add the last element with no delimiter
		oss_colorspace_names << vec_colorspace_names.back();
	}



	//Role Names
	//-----------------------------------------------
	std::vector<std::string> vec_role_names;
	for (int index = 0; index < config->getNumRoles(); index++)
	{
		//append to vector
		std::string role_name(config->getRoleName(index));
		vec_role_names.push_back(role_name);
	};

	//oss_role_names
	std::ostringstream oss_role_names;
	if (!vec_role_names.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(vec_role_names.begin(), vec_role_names.end() - 1,
			std::ostream_iterator<std::string>(oss_role_names, ", "));

		// Now add the last element with no delimiter
		oss_role_names << vec_role_names.back();
	}



	//Display Names
	//-----------------------------------------------
	std::vector<std::string> vec_display_names;
	for (int index = 0; index < config->getNumDisplays(); index++)
	{
		//append to vector
		std::string display_name(config->getDisplay(index));
		vec_display_names.push_back(display_name);
	};

	//oss_display_names
	std::ostringstream oss_display_names;
	if (!vec_display_names.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(vec_display_names.begin(), vec_display_names.end() - 1,
			std::ostream_iterator<std::string>(oss_display_names, ", "));

		// Now add the last element with no delimiter
		oss_display_names << vec_display_names.back();
	}


	



	//Format
	//-----------------------------------------------
	//separator
	std::string separator("-------------------------");
	//fmt_config_info
	boost::format fmt_config_info("Description: %s\n"
		"Working Dir: %s\n"
		"Search Path: %s\n"
		"Strict parsing enabled: %s\n"
		"OCIO version: %s\n"
		"%s\n"
		"Number of colorspaces: %s\n"
		"Colorspace Names: %s\n"
		"%s\n"
		"Number of roles: %s\n"
		"Role Names: %s\n"
		"%s\n"
		"Number of displays: %s\n"
		"Default Display: %s\n"
		"Display Names: %s\n"
		);
	fmt_config_info % config->getDescription()
		% config->getWorkingDir()
		% config->getSearchPath()
		% config->isStrictParsingEnabled()
		% OCIO::GetVersion()
		% separator
		% config->getNumColorSpaces()
		% oss_colorspace_names.str()
		% separator
		% config->getNumRoles()
		% oss_role_names.str()
		% separator
		% config->getNumDisplays()
		% config->getDefaultDisplay()
		% oss_display_names.str();



	//set number of colorspaces
	config_info = fmt_config_info.str();

	//return
	return config_info;
};




//Temp
//-----------------------------------------------
//-----------------------------------------------

//print_config_info
void OCIO_functionality::print_config_info(OCIO::ConstConfigRcPtr& config)
{
	//config exists
	if (!config)
	{
		//log
		std::cout << "Config does not exists. Cannot print info." << std::endl;
		return;
	};

	//Number color profiles
	//-----------------------------------------------
	std::cout << "Number of color profiles: " << config->getNumColorSpaces() << std::endl;

	//ColorSpace Names
	//-----------------------------------------------
	for (int index = 0; index < config->getNumColorSpaces(); index++)
	{
		//append to vector
		std::string colorspace_name(config->getColorSpaceNameByIndex(index));
		//display
		std::cout << colorspace_name << std::endl;
	};
};