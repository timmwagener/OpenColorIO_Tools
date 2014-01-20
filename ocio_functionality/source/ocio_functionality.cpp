

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

	//config
	OCIO::ConstConfigRcPtr config;
	config = OCIO::Config::CreateFromEnv();

	//config exists
	if (!config)
	{
		//log
		std::cout << "Config from env. var. does not exists" << std::endl;
	};

	return config;
};

//get_config_from_file
OCIO::ConstConfigRcPtr OCIO_functionality::get_config_from_file(std::string& file_path)
{

	//config
	OCIO::ConstConfigRcPtr config;
	config = OCIO::Config::CreateFromFile(file_path.c_str());

	//config exists
	if (!config)
	{
		//fmt_log_message
		boost::format fmt_log_message("Config from path: %s does not exists");
		fmt_log_message % file_path;

		//log
		std::cout << fmt_log_message.str() << std::endl;
	};

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