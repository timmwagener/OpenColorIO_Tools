

//Ocio_colorspace Implementation
//-----------------------------------------------
//-----------------------------------------------








//include
//-----------------------------------------------
//-----------------------------------------------
#include "ocio_colorspace.h"








//Ocio_test
//-----------------------------------------------
//-----------------------------------------------



//Static Attributes
//-----------------------------------------------
//-----------------------------------------------
MTypeId Ocio_colorspace::id(0x00002);
MString Ocio_colorspace::type_name("OCIOColorspace");

MObject Ocio_colorspace::a_verbose;

MObject Ocio_colorspace::a_input_color;
MObject Ocio_colorspace::a_output_color;

MObject Ocio_colorspace::a_env_or_file;
MObject Ocio_colorspace::a_last_env_or_file;
MObject Ocio_colorspace::a_config_file_path;
MObject Ocio_colorspace::a_last_config_file_path;
MObject Ocio_colorspace::a_internal_input_colorspace_index;
MObject Ocio_colorspace::a_internal_output_colorspace_index;





//Methods
//-----------------------------------------------
//-----------------------------------------------





//Maya
//-----------------------------------------------
//-----------------------------------------------

//Constructor
Ocio_colorspace::Ocio_colorspace() :
	config(0),
	processor(0)
{
};

//Destructor
Ocio_colorspace::~Ocio_colorspace(){};


//create
void* Ocio_colorspace::create()
{
	return new Ocio_colorspace();
}


//initialize
MStatus Ocio_colorspace::initialize()
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
	nAttr.setStorable(true);
	addAttribute(a_last_env_or_file);

	//a_config_file_path
	a_config_file_path = tAttr.create("config_file_path", "config_file_path", MFnData::kString);
	tAttr.setUsedAsFilename(true);
	tAttr.setStorable(true);
	addAttribute(a_config_file_path);

	//a_last_config_file_path
	a_last_config_file_path = tAttr.create("last_config_file_path", "last_config_file_path", MFnData::kString);
	tAttr.setStorable(true);
	addAttribute(a_last_config_file_path);

	//a_internal_input_colorspace_index
	a_internal_input_colorspace_index = nAttr.create("internal_input_colorspace_index", 
														"internal_input_colorspace_index", 
														MFnNumericData::kInt, 0);
	nAttr.setStorable(true);
	addAttribute(a_internal_input_colorspace_index);

	//a_internal_output_colorspace_index
	a_internal_output_colorspace_index = nAttr.create("internal_output_colorspace_index",
														"internal_output_colorspace_index",
														MFnNumericData::kInt, 0);
	nAttr.setStorable(true);
	addAttribute(a_internal_output_colorspace_index);



	//Attribute affects
	attributeAffects(a_input_color, a_output_color);
	attributeAffects(a_env_or_file, a_output_color);
	attributeAffects(a_config_file_path, a_output_color);

	return MStatus::kSuccess;
}

//postConstructor
void Ocio_colorspace::postConstructor()
{
	//set first execution
	first_execution = true;

	//temp
	MGlobal::displayInfo("Postconstructor executed");
};

//setDependentsDirty
MStatus Ocio_colorspace::setDependentsDirty(const MPlug& plugBeingDirtied, MPlugArray& affectedPlugs)
{
	MStatus status;
	MObject node = thisMObject();
	MFnDependencyNode fn_dep_node(node);

	//node_name
	MString node_name = this->name();

	//fmt_cmd_dgdirty
	boost::format fmt_cmd_dgdirty("dgdirty(\"%s.output_color\");");
	//format
	fmt_cmd_dgdirty % node_name.asChar();

	//input_colorspace
	if (plugBeingDirtied.partialName() == "input_colorspace")
	{
		MPlug p_output = fn_dep_node.findPlug("output_color", &status);
		if (MStatus::kSuccess == status)
		{
			//execute cmd
			MGlobal::executeCommand(MString(fmt_cmd_dgdirty.str().c_str()));
		}
	}

	//output_colorspace
	else if (plugBeingDirtied.partialName() == "output_colorspace")
	{
		MPlug p_output = fn_dep_node.findPlug("output_color", &status);
		if (MStatus::kSuccess == status)
		{
			//execute cmd
			MGlobal::executeCommand(MString(fmt_cmd_dgdirty.str().c_str()));
		}
	}
	
	

	return MStatus::kSuccess;
};

//compute
MStatus Ocio_colorspace::compute(const MPlug &plug, MDataBlock &data)
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

	
	//Update based on states
	//-----------------------------------------------
	//node_created
	if (first_execution)
		node_created();
	//env_or_file_changed
	else if (get_env_or_file() != get_last_env_or_file())
		env_or_file_changed();
	//colorspace_changed
	else if ((get_input_colorspace() != get_internal_input_colorspace_index()) || (get_output_colorspace() != get_internal_output_colorspace_index()))
		colorspace_changed();
	//config file path changed
	else if (get_config_file_path().compare(get_last_config_file_path()) != 0)
		config_file_path_changed();
	
		
	
	
	
	
	//Color conversion
	//-----------------------------------------------
	
	//vec_input_color
	MDataHandle h_input_color = data.inputValue(a_input_color);
	MFloatVector& vec_input_color = h_input_color.asFloatVector();

	//color_transform
	color_transform(vec_input_color);

	//set output color
	set_output_color(vec_input_color, data);
	
	
	
	
	
	//set first execution to false
	first_execution = false;

	//return success
	return MStatus::kSuccess;
	
}







//OCIO
//-----------------------------------------------
//-----------------------------------------------

//set_config
void Ocio_colorspace::set_config(int env_or_file)
{
	//log
	MGlobal::displayInfo(MString("Set Config."));
	
	//env
	if (env_or_file == 0)
		config = OCIO_functionality::get_config_from_env();
	//file
	else
	{
		//config_file_path
		std::string config_file_path(get_config_file_path());
		//config
		config = OCIO_functionality::get_config_from_file(config_file_path); //insert filepath here
	};
		
};

//config_exists
bool Ocio_colorspace::config_exists()
{
	//return wether or not config exists
	if (!config)
		return false;
	return true;
};

//set_colorspace_names
void Ocio_colorspace::set_colorspace_names()
{
	//log
	MGlobal::displayInfo(MString("Set Colorspace names."));
	
	//vec_colorspace_names
	vec_colorspace_names = OCIO_functionality::get_colorspace_names(config);
};

//set_processor
void Ocio_colorspace::set_processor()
{
	//config exists
	if (!config_exists())
	{
		//set processor to 0
		processor = 0;
		//log
		MGlobal::displayInfo(MString("Config does not exist. Processor not created."));
	};

	//log
	MGlobal::displayInfo(MString("Set Processor."));

	//input_colorspace_index
	int input_colorspace_index = get_input_colorspace();
	//input_colorspace_name
	std::string input_colorspace_name(OCIO_functionality::get_colorspace_name_from_index(config, input_colorspace_index));

	//output_colorspace_index
	int output_colorspace_index = get_output_colorspace();
	//output_colorspace_name
	std::string output_colorspace_name(OCIO_functionality::get_colorspace_name_from_index(config, output_colorspace_index));


	//set processor
	processor = OCIO_functionality::get_processor(config, input_colorspace_name, output_colorspace_name);
};

//color_transform
void Ocio_colorspace::color_transform(MFloatVector& vec_input_color)
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

//node_created
void Ocio_colorspace::node_created()
{

	//set_config
	set_config(get_env_or_file());
	//set_colorspace_names
	set_colorspace_names();
	//update dynamic enum attribute
	update_dynamic_enum_attribute(std::string("input_colorspace"), vec_colorspace_names);
	update_dynamic_enum_attribute(std::string("output_colorspace"), vec_colorspace_names);

	
	//colorspace_count
	int colorspace_count = OCIO_functionality::get_colorspace_count(config);
		
	//check if stored indices are smaller than colorspace count from config
	if ((get_internal_input_colorspace_index() < colorspace_count) &&
		(get_internal_output_colorspace_index() < colorspace_count))
	{
		//if so set current to stored
		set_input_colorspace(get_internal_input_colorspace_index());
		set_output_colorspace(get_internal_output_colorspace_index());
	}
	//else set them to zero
	else
	{
		set_internal_input_colorspace_index(0);
		set_internal_output_colorspace_index(0);
	};
		
	
	//set_processor
	set_processor();
};

//env_or_file_changed
void Ocio_colorspace::env_or_file_changed()
{
	//set_config
	set_config(get_env_or_file());
	//set_colorspace_names
	set_colorspace_names();
	//update dynamic enum attribute
	update_dynamic_enum_attribute(std::string("input_colorspace"), vec_colorspace_names);
	update_dynamic_enum_attribute(std::string("output_colorspace"), vec_colorspace_names);

	//set internal colorspace indices to 0
	set_internal_input_colorspace_index(0);
	set_internal_output_colorspace_index(0);
	//set_last_env_or_file
	set_last_env_or_file(get_env_or_file());

	
	//set_processor
	set_processor();

};

//colorspace_changed
void Ocio_colorspace::colorspace_changed()
{
	//set internal colorspace indices
	set_internal_input_colorspace_index(get_input_colorspace());
	set_internal_output_colorspace_index(get_output_colorspace());
	
	//set_processor
	set_processor();
};

//config_file_path_changed
void Ocio_colorspace::config_file_path_changed()
{
	//if not env_or_file return
	if (!get_env_or_file())
		return;

	//set_config
	set_config(get_env_or_file());
	//set_colorspace_names
	set_colorspace_names();
	//update dynamic enum attribute
	update_dynamic_enum_attribute(std::string("input_colorspace"), vec_colorspace_names);
	update_dynamic_enum_attribute(std::string("output_colorspace"), vec_colorspace_names);

	//set internal colorspace indices to 0
	set_internal_input_colorspace_index(0);
	set_internal_output_colorspace_index(0);

	//set last config_file_path to current path
	set_last_config_file_path(get_config_file_path());

	//set_processor
	set_processor();

};




//Own
//-----------------------------------------------
//-----------------------------------------------

//get_plug
MPlug Ocio_colorspace::get_plug(std::string attr_name)
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
bool Ocio_colorspace::attribute_exists(std::string attr_name)
{
	//p_plug
	MPlug p_plug = get_plug(attr_name);
	
	return !p_plug.isNull();

};

//get_env_or_file
int Ocio_colorspace::get_env_or_file()
{
	//p_env_or_file
	MPlug p_env_or_file = get_plug(std::string("env_or_file"));

	return p_env_or_file.asInt();
};

//get_last_env_or_file
int Ocio_colorspace::get_last_env_or_file()
{
	//p_last_env_or_file
	MPlug p_last_env_or_file = get_plug(std::string("last_env_or_file"));

	return p_last_env_or_file.asInt();
};

//set_last_env_or_file
void Ocio_colorspace::set_last_env_or_file(int value)
{
	//p_last_env_or_file
	MPlug p_last_env_or_file = get_plug(std::string("last_env_or_file"));

	p_last_env_or_file.setInt(value);
};

//get_config_file_path
std::string Ocio_colorspace::get_config_file_path()
{
	//p_config_file_path
	MPlug p_config_file_path = get_plug(std::string("config_file_path"));
	
	//ms_config_file_path
	MString ms_config_file_path = p_config_file_path.asString();

	//std_config_file_path
	std::string std_config_file_path(ms_config_file_path.asChar());

	return std_config_file_path;
};

//get_last_config_file_path
std::string Ocio_colorspace::get_last_config_file_path()
{
	//p_last_config_file_path
	MPlug p_last_config_file_path = get_plug(std::string("last_config_file_path"));

	//ms_last_config_file_path
	MString ms_last_config_file_path = p_last_config_file_path.asString();

	//std_last_config_file_path
	std::string std_last_config_file_path(ms_last_config_file_path.asChar());

	return std_last_config_file_path;
};

//set_last_config_file_path
void Ocio_colorspace::set_last_config_file_path(std::string new_path)
{
	//p_last_config_file_path
	MPlug p_last_config_file_path = get_plug(std::string("last_config_file_path"));
	//set plug
	p_last_config_file_path.setString(MString(new_path.c_str()));
	
};

//attr_connected
bool Ocio_colorspace::attr_connected(std::string attr_name)
{
	//p_plug
	MPlug p_plug = get_plug(attr_name);

	//return
	return p_plug.isConnected();
};

//set_output_color
void Ocio_colorspace::set_output_color(MFloatVector& vec_color, MDataBlock& data)
{
	MDataHandle h_output_color = data.outputValue(a_output_color);
	MFloatVector& vec_output_color = h_output_color.asFloatVector();
	vec_output_color = vec_color;

	//clean handle
	h_output_color.setClean();

	//tmp
	//MGlobal::displayInfo("Set output color attribute");
};











//Colorspace indices
//-----------------------------------------------
//-----------------------------------------------

//get_input_colorspace
int Ocio_colorspace::get_input_colorspace()
{
	//p_input_colorspace
	MPlug p_input_colorspace = get_plug(std::string("input_colorspace"));
	return p_input_colorspace.asInt();
};

//set_input_colorspace
void Ocio_colorspace::set_input_colorspace(int index)
{
	//p_input_colorspace
	MPlug p_input_colorspace = get_plug(std::string("input_colorspace"));
	p_input_colorspace.setInt(index);
};

//get_output_colorspace
int Ocio_colorspace::get_output_colorspace()
{
	//p_output_colorspace
	MPlug p_output_colorspace = get_plug(std::string("output_colorspace"));
	return p_output_colorspace.asInt();
};

//set_output_colorspace
void Ocio_colorspace::set_output_colorspace(int index)
{
	//p_output_colorspace
	MPlug p_output_colorspace = get_plug(std::string("output_colorspace"));
	p_output_colorspace.setInt(index);
};

//get_internal_input_colorspace_index
int Ocio_colorspace::get_internal_input_colorspace_index()
{
	//p_internal_input_colorspace_index
	MPlug p_internal_input_colorspace_index = get_plug(std::string("internal_input_colorspace_index"));

	return p_internal_input_colorspace_index.asInt();
};

//set_internal_input_colorspace_index
void Ocio_colorspace::set_internal_input_colorspace_index(int index)
{
	//p_internal_input_colorspace_index
	MPlug p_internal_input_colorspace_index = get_plug(std::string("internal_input_colorspace_index"));
	p_internal_input_colorspace_index.setInt(index);
};

//get_internal_output_colorspace_index
int Ocio_colorspace::get_internal_output_colorspace_index()
{
	//p_internal_output_colorspace_index
	MPlug p_internal_output_colorspace_index = get_plug(std::string("internal_output_colorspace_index"));
	return p_internal_output_colorspace_index.asInt();
};

//set_internal_output_colorspace_index
void Ocio_colorspace::set_internal_output_colorspace_index(int index)
{
	//p_internal_output_colorspace_index
	MPlug p_internal_output_colorspace_index = get_plug(std::string("internal_output_colorspace_index"));
	p_internal_output_colorspace_index.setInt(index);
};








//Enum
//-----------------------------------------------
//-----------------------------------------------

//create_dynamic_enum_attribute
void Ocio_colorspace::create_dynamic_enum_attribute(std::string attribute_name,
													std::vector<std::string>& value_list)
{
	//eAttr
	MFnEnumAttribute eAttr;

	//a_attribute
	MObject a_attribute = eAttr.create(attribute_name.c_str(), attribute_name.c_str(), 0);
	eAttr.setStorable(true);
	//iterate and add values
	for (int index = 0; index < value_list.size(); index++)
		eAttr.addField(value_list[index].c_str(), index);

	//node
	MObject node = thisMObject();

	//dg_modifier
	MDGModifier dg_modifier;
	dg_modifier.addAttribute(node, a_attribute);
	dg_modifier.doIt();

	//update AE
	MGlobal::executeCommandOnIdle(MString("setLocalView \"\" \"\" 1;"));

	//tmp
	MGlobal::displayInfo(MString("Added dynamic enum attr."));
};


//remove_dynamic_attribute
void Ocio_colorspace::remove_dynamic_attribute(std::string attribute_name)
{
	//p_attribute
	MPlug p_attribute = get_plug(attribute_name);
	//o_attribute
	MObject a_attribute = p_attribute.attribute();

	//node
	MObject node = thisMObject();

	//dg_modifier
	MDGModifier dg_modifier;
	dg_modifier.removeAttribute(node, a_attribute);
	dg_modifier.doIt();

	//tmp
	MGlobal::displayInfo(MString("Removed dynamic attr.:") + attribute_name.c_str());
};

//update_dynamic_enum_attribute
void Ocio_colorspace::update_dynamic_enum_attribute(std::string attribute_name,
										std::vector<std::string>& value_list)
{
	//remove_dynamic_attribute
	if (attribute_exists(attribute_name))
		remove_dynamic_attribute(attribute_name);

	//create_dynamic_enum_attribute
	create_dynamic_enum_attribute(attribute_name,
									value_list);

};





//Temp
//-----------------------------------------------
//-----------------------------------------------

//print_config_info
void Ocio_colorspace::print_config_info()
{
	OCIO_functionality::print_config_info(config);
};