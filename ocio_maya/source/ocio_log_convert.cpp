

//Ocio_log_convert Implementation
//-----------------------------------------------
//-----------------------------------------------








//include
//-----------------------------------------------
//-----------------------------------------------
#include "ocio_log_convert.h"








//Ocio_test
//-----------------------------------------------
//-----------------------------------------------



//Static Attributes
//-----------------------------------------------
//-----------------------------------------------
MTypeId Ocio_log_convert::id(0x00004);
MString Ocio_log_convert::type_name("OCIOLogConvert");

MObject Ocio_log_convert::a_verbose;

MObject Ocio_log_convert::a_input_color;
MObject Ocio_log_convert::a_output_color;

MObject Ocio_log_convert::a_env_or_file;
MObject Ocio_log_convert::a_last_env_or_file;
MObject Ocio_log_convert::a_config_file_path;
MObject Ocio_log_convert::a_last_config_file_path;
MObject Ocio_log_convert::a_operation;
MObject Ocio_log_convert::a_last_operation;







//Methods
//-----------------------------------------------
//-----------------------------------------------





//Maya
//-----------------------------------------------
//-----------------------------------------------

//Constructor
Ocio_log_convert::Ocio_log_convert() :
	processor(0)
{
};

//Destructor
Ocio_log_convert::~Ocio_log_convert(){};


//create
void* Ocio_log_convert::create()
{
	return new Ocio_log_convert();
}


//initialize
MStatus Ocio_log_convert::initialize()
{

	//MFnSets
	MFnEnumAttribute eAttr;
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;


	//a_verbose
	a_verbose = eAttr.create("verbose", "verbose", 1);
	eAttr.addField("no verbose", 0);
	eAttr.addField("verbose", 1);
	addAttribute(a_verbose);

	//a_input_color
	a_input_color = nAttr.createColor("input_color", "input_color");
	nAttr.setUsedAsColor(true);
	nAttr.setStorable(true);
	addAttribute(a_input_color);

	//a_output_color
	a_output_color = nAttr.createColor("output_color", "output_color");
	nAttr.setUsedAsColor(true);
	nAttr.setStorable(false);
	addAttribute(a_output_color);

	//a_env_or_file
	a_env_or_file = eAttr.create("env_or_file", "env_or_file", 0);
	eAttr.addField("OCIO Environment Variable", 0);
	eAttr.addField("Custom Path", 1);
	eAttr.setStorable(true);
	addAttribute(a_env_or_file);

	//a_last_env_or_file
	a_last_env_or_file = nAttr.create("last_env_or_file", "last_env_or_file", MFnNumericData::kInt, 0);
	nAttr.setHidden(true);
	nAttr.setStorable(true);
	addAttribute(a_last_env_or_file);

	//a_config_file_path
	a_config_file_path = tAttr.create("config_file_path", "config_file_path", MFnData::kString);
	tAttr.setUsedAsFilename(true);
	tAttr.setStorable(true);
	addAttribute(a_config_file_path);

	//a_last_config_file_path
	a_last_config_file_path = tAttr.create("last_config_file_path", "last_config_file_path", MFnData::kString);
	tAttr.setHidden(true);
	tAttr.setStorable(true);
	addAttribute(a_last_config_file_path);

	//a_operation
	a_operation = eAttr.create("operation", "operation", 0);
	eAttr.addField("Log to Lin", 0);
	eAttr.addField("Lin to Log", 1);
	addAttribute(a_operation);
	
	//a_last_operation
	a_last_operation = nAttr.create("last_operation", "last_operation", MFnNumericData::kInt, 0);
	nAttr.setHidden(true);
	nAttr.setStorable(true);
	addAttribute(a_last_operation);

	
	
	
	
	
	//Attribute affects
	attributeAffects(a_input_color, a_output_color);
	attributeAffects(a_env_or_file, a_output_color);
	attributeAffects(a_config_file_path, a_output_color);
	attributeAffects(a_operation, a_output_color);
	



	return MStatus::kSuccess;
}

//postConstructor
void Ocio_log_convert::postConstructor()
{
	//set first execution
	first_execution = true;

	//log
	MGlobal::displayInfo("Postconstructor executed");
};

//compute
MStatus Ocio_log_convert::compute(const MPlug &plug, MDataBlock &data)
{
	
	//wrong plug requested
	//-----------------------------------------------
	//-----------------------------------------------
	if ((plug != a_output_color) && (plug.parent() != a_output_color))
	{
		return MStatus::kUnknownParameter;
	}

	
	
	
	
	//right plug requested
	//-----------------------------------------------
	//-----------------------------------------------


	//Get attributes
	//-----------------------------------------------

	//env_or_file
	int env_or_file = data.inputValue(a_env_or_file).asInt();
	//last_env_or_file
	int last_env_or_file = data.inputValue(a_last_env_or_file).asInt();

	//config_file_path
	std::string config_file_path = get_string_attribute(std::string("config_file_path"));
	//last_config_file_path
	std::string last_config_file_path = get_string_attribute(std::string("last_config_file_path"));

	//operation
	int operation = data.inputValue(a_operation).asInt();
	//last_operation
	int last_operation = data.inputValue(a_last_operation).asInt();
	
	




	
	
	
	//Set processor
	//-----------------------------------------------

	//first_execution
	if (first_execution)
		set_processor(env_or_file,
		config_file_path,
		operation);
	//env_or_file
	else if (env_or_file != last_env_or_file)
		set_processor(env_or_file,
		config_file_path,
		operation);
	//config_file_path
	else if (config_file_path.compare(last_config_file_path) != 0)
		set_processor(env_or_file,
		config_file_path,
		operation);
	//operation
	else if (operation != last_operation)
		set_processor(env_or_file,
		config_file_path,
		operation);
	




	
	
	//Color conversion
	//-----------------------------------------------
	
	//vec_input_color
	MDataHandle h_input_color = data.inputValue(a_input_color);
	MFloatVector& vec_input_color = h_input_color.asFloatVector();

	//transform_color
	if (processor)
		color_transform(vec_input_color);

	//set output color
	set_output_color(vec_input_color, data);
	
	




	
	//Set attributes
	//-----------------------------------------------

	//last_env_or_file
	set_int_attribute("last_env_or_file", env_or_file);
	//last_config_file_path
	set_string_attribute("last_config_file_path", config_file_path);
	//last_operation
	set_int_attribute("last_operation", operation);
	
	
	
	//set first execution to false
	first_execution = false;

	//return success
	return MStatus::kSuccess;
	
}







//OCIO
//-----------------------------------------------
//-----------------------------------------------

//set_processor
void Ocio_log_convert::set_processor(int env_or_file,
										std::string config_file_path,
										int operation)
{
	
	
	//set processor
	processor = OCIO_functionality::get_processor(env_or_file,
													config_file_path,
													operation);
	//msg_processor
	MString msg_processor("Processor set");
	//msg_empty_config_path
	MString msg_empty_config_path("No config file path set. No transformation will be applied. ");

	//display msg
	if ((!config_file_path.size()) && (env_or_file == 1))
		MGlobal::displayWarning(msg_empty_config_path + msg_processor);
	else
		MGlobal::displayInfo(msg_processor);

};

//color_transform
void Ocio_log_convert::color_transform(MFloatVector& vec_input_color)
{
	//pixels
	float pixels[3];
	vec_input_color.get(pixels);

	//pixel_r, pixel_g, pixel_b;
	float* pixel_r = &pixels[0];
	float* pixel_g = &pixels[1];
	float* pixel_b = &pixels[2];

	//convert
	OCIO_functionality::color_transform_single_pixel(pixel_r, pixel_g, pixel_b, processor);

	//set colors back to vec_input_color
	vec_input_color.x = pixels[0];
	vec_input_color.y = pixels[1];
	vec_input_color.z = pixels[2];
}







//States
//-----------------------------------------------
//-----------------------------------------------












//Own
//-----------------------------------------------
//-----------------------------------------------

//get_string_attribute
std::string Ocio_log_convert::get_string_attribute(std::string attr_name)
{
	//p_attr
	MPlug p_attr = get_plug(attr_name);

	//ms_plug_value
	MString ms_plug_value = p_attr.asString();

	//std_plug_value
	std::string std_plug_value(ms_plug_value.asChar());

	return std_plug_value;
};

//set_string_attribute
void Ocio_log_convert::set_string_attribute(std::string attr_name,
														std::string value)
{
	//p_attr
	MPlug p_attr = get_plug(attr_name);

	//ms_value
	MString ms_value(value.c_str());

	//set value
	p_attr.setString(ms_value);
};

//set_int_attribute
void Ocio_log_convert::set_int_attribute(std::string attr_name,
												int value)
{
	//p_attr
	MPlug p_attr = get_plug(attr_name);

	//set value
	p_attr.setInt(value);
};









//Helper
//-----------------------------------------------
//-----------------------------------------------

//get_plug
MPlug Ocio_log_convert::get_plug(std::string attr_name)
{
	//node
	MObject node = thisMObject();
	//fn_dep_node
	MFnDependencyNode fn_dep_node(node);
	//p_plug
	MPlug p_plug = fn_dep_node.findPlug(attr_name.c_str(), true);

	return p_plug;
};

//attribute_exists
bool Ocio_log_convert::attribute_exists(std::string attr_name)
{
	//p_plug
	MPlug p_plug = get_plug(attr_name);
	
	return !p_plug.isNull();

};

//attr_connected
bool Ocio_log_convert::attr_connected(std::string attr_name)
{
	//p_plug
	MPlug p_plug = get_plug(attr_name);

	//return
	return p_plug.isConnected();
};

//set_output_color
void Ocio_log_convert::set_output_color(MFloatVector& vec_color, MDataBlock& data)
{
	MDataHandle h_output_color = data.outputValue(a_output_color);
	MFloatVector& vec_output_color = h_output_color.asFloatVector();
	vec_output_color = vec_color;

	//clean handle
	h_output_color.setClean();

	//tmp
	//MGlobal::displayInfo("Set output color attribute");
};

//get_instance_name
std::string Ocio_log_convert::get_instance_name()
{
	
	//ms_node_name
	MString ms_node_name = this->name();

	//node_name
	std::string node_name = std::string(ms_node_name.asChar());

	return node_name;
};













//Temp
//-----------------------------------------------
//-----------------------------------------------

