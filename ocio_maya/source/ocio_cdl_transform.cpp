

//Ocio_cdl_transform Implementation
//-----------------------------------------------
//-----------------------------------------------








//include
//-----------------------------------------------
//-----------------------------------------------
#include "ocio_cdl_transform.h"








//Ocio_test
//-----------------------------------------------
//-----------------------------------------------



//Static Attributes
//-----------------------------------------------
//-----------------------------------------------
MTypeId Ocio_cdl_transform::id(0x00005);
MString Ocio_cdl_transform::type_name("OCIOCDLTransform");

MObject Ocio_cdl_transform::a_verbose;

MObject Ocio_cdl_transform::a_input_color;
MObject Ocio_cdl_transform::a_output_color;

MObject Ocio_cdl_transform::a_slope;
MObject Ocio_cdl_transform::a_last_slope;
MObject Ocio_cdl_transform::a_offset;
MObject Ocio_cdl_transform::a_last_offset;
MObject Ocio_cdl_transform::a_power;
MObject Ocio_cdl_transform::a_last_power;
MObject Ocio_cdl_transform::a_saturation;
MObject Ocio_cdl_transform::a_last_saturation;
MObject Ocio_cdl_transform::a_direction;
MObject Ocio_cdl_transform::a_last_direction;







//Methods
//-----------------------------------------------
//-----------------------------------------------





//Maya
//-----------------------------------------------
//-----------------------------------------------

//Constructor
Ocio_cdl_transform::Ocio_cdl_transform() :
	processor(0)
{
};

//Destructor
Ocio_cdl_transform::~Ocio_cdl_transform(){};


//create
void* Ocio_cdl_transform::create()
{
	return new Ocio_cdl_transform();
}


//initialize
MStatus Ocio_cdl_transform::initialize()
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

	//a_slope
	a_slope = nAttr.createColor("slope", "slope");
	nAttr.setUsedAsColor(true);
	nAttr.setStorable(true);
	nAttr.setDefault(1.0, 1.0, 1.0);
	addAttribute(a_slope);

	//a_last_slope
	a_last_slope = nAttr.createColor("last_slope", "last_slope");
	nAttr.setUsedAsColor(true);
	nAttr.setStorable(true);
	nAttr.setDefault(1.0, 1.0, 1.0);
	addAttribute(a_last_slope);
	
	//a_offset
	a_offset = nAttr.createColor("offset", "offset");
	nAttr.setUsedAsColor(true);
	nAttr.setStorable(true);
	nAttr.setDefault(0, 0, 0);
	addAttribute(a_offset);

	//a_last_offset
	a_last_offset = nAttr.createColor("last_offset", "last_offset");
	nAttr.setUsedAsColor(true);
	nAttr.setStorable(true);
	nAttr.setDefault(0, 0, 0);
	addAttribute(a_last_offset);
	
	//a_power
	a_power = nAttr.createColor("power", "power");
	nAttr.setUsedAsColor(true);
	nAttr.setStorable(true);
	nAttr.setDefault(1.0, 1.0, 1.0);
	addAttribute(a_power);

	//a_last_power
	a_last_power = nAttr.createColor("last_power", "last_power");
	nAttr.setUsedAsColor(true);
	nAttr.setStorable(true);
	nAttr.setDefault(1.0, 1.0, 1.0);
	addAttribute(a_last_power);
	
	//a_saturation
	a_saturation = nAttr.create("saturation", "saturation", MFnNumericData::kFloat, 1.0);
	nAttr.setStorable(true);
	addAttribute(a_saturation);

	//a_last_saturation
	a_last_saturation = nAttr.create("last_saturation", "last_saturation", MFnNumericData::kFloat, 1.0);
	nAttr.setStorable(true);
	addAttribute(a_last_saturation);
	
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
	
	
	
	
	//Attribute affects
	attributeAffects(a_input_color, a_output_color);
	attributeAffects(a_slope, a_output_color);
	attributeAffects(a_offset, a_output_color);
	attributeAffects(a_power, a_output_color);
	attributeAffects(a_saturation, a_output_color);
	attributeAffects(a_direction, a_output_color);
	



	return MStatus::kSuccess;
}

//postConstructor
void Ocio_cdl_transform::postConstructor()
{
	//set first execution
	first_execution = true;

	//log
	MGlobal::displayInfo("Postconstructor executed");
};

//compute
MStatus Ocio_cdl_transform::compute(const MPlug &plug, MDataBlock &data)
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
	
	MFloatVector vec_slope, vec_last_slope, vec_offset, vec_last_offset, vec_power, vec_last_power;
	get_sop_as_mfloatvectors(vec_slope,
								vec_last_slope,
								vec_offset,
								vec_last_offset,
								vec_power,
								vec_last_power,
								data);

	//saturation
	MDataHandle h_saturation = data.inputValue(a_saturation);
	float saturation = h_saturation.asFloat();
	//last_saturation
	MDataHandle h_last_saturation = data.inputValue(a_last_saturation);
	float last_saturation = h_last_saturation.asFloat();

	//direction
	MDataHandle h_direction = data.inputValue(a_direction);
	int direction = h_direction.asInt();
	//last_direction
	MDataHandle h_last_direction = data.inputValue(a_last_direction);
	int last_direction = h_last_direction.asInt();

	
	
	




	
	
	
	//Set processor
	//-----------------------------------------------

	if (first_execution)
	{
		float* sop_ptr = new float[9];
		get_sop_as_float_ptr(sop_ptr, vec_slope, vec_offset, vec_power);
		set_processor(sop_ptr, saturation, direction);
		delete[] sop_ptr;

	}
	else if (vec_slope != vec_last_slope)
	{
		float* sop_ptr = new float[9];
		get_sop_as_float_ptr(sop_ptr, vec_slope, vec_offset, vec_power);
		set_processor(sop_ptr, saturation, direction);
		delete[] sop_ptr;
		set_color_attribute(std::string("last_slope"), vec_slope);
	}
	else if (vec_offset != vec_last_offset)
	{
		float* sop_ptr = new float[9];
		get_sop_as_float_ptr(sop_ptr, vec_slope, vec_offset, vec_power);
		set_processor(sop_ptr, saturation, direction);
		delete[] sop_ptr;
		set_color_attribute(std::string("last_offset"), vec_offset);
	}
	else if (vec_power != vec_last_power)
	{
		float* sop_ptr = new float[9];
		get_sop_as_float_ptr(sop_ptr, vec_slope, vec_offset, vec_power);
		set_processor(sop_ptr, saturation, direction);
		delete[] sop_ptr;
		set_color_attribute(std::string("last_power"), vec_power);
	}
	else if (saturation != last_saturation)
	{
		float* sop_ptr = new float[9];
		get_sop_as_float_ptr(sop_ptr, vec_slope, vec_offset, vec_power);
		set_processor(sop_ptr, saturation, direction);
		delete[] sop_ptr;
		set_float_attribute(std::string("last_saturation"), saturation);
	}
	else if (direction != last_direction)
	{
		float* sop_ptr = new float[9];
		get_sop_as_float_ptr(sop_ptr, vec_slope, vec_offset, vec_power);
		set_processor(sop_ptr, saturation, direction);
		delete[] sop_ptr;
		set_int_attribute(std::string("last_direction"), direction);
	}
	




	
	
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
	
	




	
	

	
	
	
	
	//set first execution to false
	first_execution = false;

	//return success
	return MStatus::kSuccess;
	
}







//OCIO
//-----------------------------------------------
//-----------------------------------------------

//set_processor
void Ocio_cdl_transform::set_processor(float*& sop, float saturation, int direction)
{
	
	
	//set processor
	processor = OCIO_functionality::get_processor_from_cdl_transform(sop, saturation, direction);

	//display msg
	if (!processor)
		MGlobal::displayWarning("Processor empty");
	else
		MGlobal::displayInfo("Processor set");

};

//color_transform
void Ocio_cdl_transform::color_transform(MFloatVector& vec_input_color)
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

void Ocio_cdl_transform::get_sop_as_mfloatvectors(MFloatVector& vec_slope,
													MFloatVector& vec_last_slope,
													MFloatVector& vec_offset,
													MFloatVector& vec_last_offset,
													MFloatVector& vec_power,
													MFloatVector& vec_last_power, 
													MDataBlock& data)
{
	//vec_slope
	MDataHandle h_slope = data.inputValue(a_slope);
	vec_slope = h_slope.asFloatVector();
	//vec_last_slope
	MDataHandle h_last_slope = data.inputValue(a_last_slope);
	vec_last_slope = h_last_slope.asFloatVector();

	//vec_offset
	MDataHandle h_offset = data.inputValue(a_offset);
	vec_offset = h_offset.asFloatVector();
	//vec_last_offset
	MDataHandle h_last_offset = data.inputValue(a_last_offset);
	vec_last_offset = h_last_offset.asFloatVector();

	//vec_power
	MDataHandle h_power = data.inputValue(a_power);
	vec_power = h_power.asFloatVector();
	//vec_last_power
	MDataHandle h_last_power = data.inputValue(a_last_power);
	vec_last_power = h_last_power.asFloatVector();
};

//get_sop_as_float_ptr
void Ocio_cdl_transform::get_sop_as_float_ptr(float*& sop_ptr,
							MFloatVector& vec_slope,
							MFloatVector& vec_offset,
							MFloatVector& vec_power)
{
	//set sop_ptr
	sop_ptr[0] = vec_slope.x;
	sop_ptr[1] = vec_slope.y;
	sop_ptr[2] = vec_slope.z;

	sop_ptr[3] = vec_offset.x;
	sop_ptr[4] = vec_offset.y;
	sop_ptr[5] = vec_offset.z;

	sop_ptr[6] = vec_power.x;
	sop_ptr[7] = vec_power.y;
	sop_ptr[8] = vec_power.z;
};

//set_color_attribute
void Ocio_cdl_transform::set_color_attribute(std::string attr_name, MFloatVector& color)
{
	//p_plug
	MPlug p_plug = get_plug(attr_name);

	//set color
	p_plug.child(0).setFloat(color.x);
	p_plug.child(1).setFloat(color.y);
	p_plug.child(2).setFloat(color.z);

}

//set_int_attribute
void Ocio_cdl_transform::set_int_attribute(std::string attr_name, int value)
{
	//p_attr
	MPlug p_attr = get_plug(attr_name);

	//set value
	p_attr.setInt(value);
};

//set_float_attribute
void Ocio_cdl_transform::set_float_attribute(std::string attr_name, float value)
{
	//p_attr
	MPlug p_attr = get_plug(attr_name);

	//set value
	p_attr.setFloat(value);
};




//Helper
//-----------------------------------------------
//-----------------------------------------------

//get_plug
MPlug Ocio_cdl_transform::get_plug(std::string attr_name)
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
bool Ocio_cdl_transform::attribute_exists(std::string attr_name)
{
	//p_plug
	MPlug p_plug = get_plug(attr_name);
	
	return !p_plug.isNull();

};

//attr_connected
bool Ocio_cdl_transform::attr_connected(std::string attr_name)
{
	//p_plug
	MPlug p_plug = get_plug(attr_name);

	//return
	return p_plug.isConnected();
};

//set_output_color
void Ocio_cdl_transform::set_output_color(MFloatVector& vec_color, MDataBlock& data)
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
std::string Ocio_cdl_transform::get_instance_name()
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

