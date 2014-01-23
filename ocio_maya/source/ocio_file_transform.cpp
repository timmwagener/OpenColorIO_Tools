

//Ocio_file_transform Implementation
//-----------------------------------------------
//-----------------------------------------------








//include
//-----------------------------------------------
//-----------------------------------------------
#include "ocio_file_transform.h"








//Ocio_test
//-----------------------------------------------
//-----------------------------------------------



//Static Attributes
//-----------------------------------------------
//-----------------------------------------------
MTypeId Ocio_file_transform::id(0x00003);
MString Ocio_file_transform::type_name("OCIOFileTransform");

MObject Ocio_file_transform::a_verbose;

MObject Ocio_file_transform::a_input_color;
MObject Ocio_file_transform::a_output_color;

MObject Ocio_file_transform::a_lut_file_path;
MObject Ocio_file_transform::a_last_lut_file_path;
MObject Ocio_file_transform::a_cccid;
MObject Ocio_file_transform::a_last_cccid;
MObject Ocio_file_transform::a_direction;
MObject Ocio_file_transform::a_last_direction;
MObject Ocio_file_transform::a_interpolation;
MObject Ocio_file_transform::a_last_interpolation;





//Methods
//-----------------------------------------------
//-----------------------------------------------





//Maya
//-----------------------------------------------
//-----------------------------------------------

//Constructor
Ocio_file_transform::Ocio_file_transform() :
	processor(0)
{
};

//Destructor
Ocio_file_transform::~Ocio_file_transform(){};


//create
void* Ocio_file_transform::create()
{
	return new Ocio_file_transform();
}


//initialize
MStatus Ocio_file_transform::initialize()
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

	//a_lut_file_path
	a_lut_file_path = tAttr.create("lut_file_path", "lut_file_path", MFnData::kString);
	tAttr.setUsedAsFilename(true);
	tAttr.setStorable(true);
	addAttribute(a_lut_file_path);

	//a_last_lut_file_path
	a_last_lut_file_path = tAttr.create("last_lut_file_path", "last_lut_file_path", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setHidden(true);
	addAttribute(a_last_lut_file_path);

	//a_cccid
	a_cccid = tAttr.create("cccid", "cccid", MFnData::kString);
	tAttr.setStorable(true);
	addAttribute(a_cccid);

	//a_last_cccid
	a_last_cccid = tAttr.create("last_cccid", "last_cccid", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setHidden(true);
	addAttribute(a_last_cccid);
	
	//a_direction
	a_direction = eAttr.create("direction", "direction", 0);
	eAttr.addField("Forward", 0);
	eAttr.addField("Inverse", 1);
	eAttr.setStorable(true);
	addAttribute(a_direction);

	//a_last_direction
	a_last_direction = nAttr.create("last_direction", "last_direction", MFnNumericData::kInt, 0);
	nAttr.setStorable(true);
	nAttr.setHidden(true);
	addAttribute(a_last_direction);

	//a_interpolation
	a_interpolation = eAttr.create("interpolation", "interpolation", 1);
	eAttr.addField("Nearest", 0);
	eAttr.addField("Linear", 1);
	eAttr.addField("Tetrahedral", 2);
	eAttr.addField("Best", 3);
	eAttr.setStorable(true);
	addAttribute(a_interpolation);

	//a_last_interpolation
	a_last_interpolation = nAttr.create("last_interpolation", "last_interpolation", MFnNumericData::kInt, 0);
	nAttr.setStorable(true);
	nAttr.setHidden(true);
	addAttribute(a_last_interpolation);

	//Attribute affects
	attributeAffects(a_input_color, a_output_color);
	attributeAffects(a_lut_file_path, a_output_color);
	attributeAffects(a_cccid, a_output_color);
	attributeAffects(a_direction, a_output_color);
	attributeAffects(a_interpolation, a_output_color);



	return MStatus::kSuccess;
}

//postConstructor
void Ocio_file_transform::postConstructor()
{
	//set first execution
	first_execution = true;

	//log
	MGlobal::displayInfo("Postconstructor executed");
};

//compute
MStatus Ocio_file_transform::compute(const MPlug &plug, MDataBlock &data)
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

	//lut_file_path
	std::string lut_file_path = get_string_attribute(std::string("lut_file_path"));
	//last_lut_file_path
	std::string last_lut_file_path = get_string_attribute(std::string("last_lut_file_path"));

	//cccid
	std::string cccid = get_string_attribute(std::string("cccid"));
	//last_cccid
	std::string last_cccid = get_string_attribute(std::string("last_cccid"));

	//direction
	int direction = data.inputValue(a_direction).asInt();
	//last_direction
	int last_direction = data.inputValue(a_last_direction).asInt();

	//interpolation
	int interpolation = data.inputValue(a_interpolation).asInt();
	//last_interpolation
	int last_interpolation = data.inputValue(a_last_interpolation).asInt();






	//Set processor
	//-----------------------------------------------

	//first_execution
	if (first_execution)
		set_processor(lut_file_path,
						cccid,
						direction,
						interpolation);
	//lut_file_path
	else if (lut_file_path.compare(last_lut_file_path) != 0)
		set_processor(lut_file_path,
						cccid,
						direction,
						interpolation);
	//cccid
	else if (cccid.compare(last_cccid) != 0)
		set_processor(lut_file_path,
						cccid,
						direction,
						interpolation);
	//direction
	else if (direction != last_direction)
		set_processor(lut_file_path,
						cccid,
						direction,
						interpolation);
	//interpolation
	else if (interpolation != last_interpolation)
		set_processor(lut_file_path,
						cccid,
						direction,
						interpolation);
	
	

	
	
	
	
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

	//last_lut_file_path
	set_string_attribute("last_lut_file_path", lut_file_path);
	//cccid
	set_string_attribute("last_cccid", cccid);
	//direction
	set_int_attribute("last_direction", direction);
	//interpolation
	set_int_attribute("last_interpolation", interpolation);
	
	
	
	
	//set first execution to false
	first_execution = false;

	//return success
	return MStatus::kSuccess;
	
}







//OCIO
//-----------------------------------------------
//-----------------------------------------------

//set_processor
void Ocio_file_transform::set_processor(std::string lut_file_path_value, 
										std::string cccid_value,
										int direction_value,
										int interpolation_value)
{
	
	
	//set processor
	processor = OCIO_functionality::get_processor_from_file_transform(lut_file_path_value,
																		cccid_value,
																		direction_value,
																		interpolation_value);
	//msg_processor
	MString msg_processor("Processor set");
	//msg_empty_lut_path
	MString msg_empty_lut_path("No Lut file path set. No transformation will be applied. ");

	//display msg
	if (!lut_file_path_value.size())
		MGlobal::displayWarning(msg_empty_lut_path + msg_processor);
	else
		MGlobal::displayInfo(msg_processor);

};

//color_transform
void Ocio_file_transform::color_transform(MFloatVector& vec_input_color)
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
std::string Ocio_file_transform::get_string_attribute(std::string attr_name)
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
void Ocio_file_transform::set_string_attribute(std::string attr_name, 
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
void Ocio_file_transform::set_int_attribute(std::string attr_name,
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
MPlug Ocio_file_transform::get_plug(std::string attr_name)
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
bool Ocio_file_transform::attribute_exists(std::string attr_name)
{
	//p_plug
	MPlug p_plug = get_plug(attr_name);
	
	return !p_plug.isNull();

};

//attr_connected
bool Ocio_file_transform::attr_connected(std::string attr_name)
{
	//p_plug
	MPlug p_plug = get_plug(attr_name);

	//return
	return p_plug.isConnected();
};

//set_output_color
void Ocio_file_transform::set_output_color(MFloatVector& vec_color, MDataBlock& data)
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
std::string Ocio_file_transform::get_instance_name()
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

