

//Ocio_simple_transform Implementation
//-----------------------------------------------


//include
//-----------------------------------------------
#include "ocio_simple_transform.h"


//Ocio_test
//-----------------------------------------------

//Attributes
MTypeId Ocio_simple_transform::id(0x00001);
MString Ocio_simple_transform::type_name("ocio_simple_transform");

MObject Ocio_simple_transform::a_verbose;
MObject Ocio_simple_transform::a_input_color;
MObject Ocio_simple_transform::a_output_color;


//Constructor / Deconstructor
//-----------------------------------------------
Ocio_simple_transform::Ocio_simple_transform():
	config(0),
	processor(0)
{
};
Ocio_simple_transform::~Ocio_simple_transform(){};


//create
//-----------------------------------------------
void* Ocio_simple_transform::create()
{
	return new Ocio_simple_transform();
}


//initialize
//-----------------------------------------------
MStatus Ocio_simple_transform::initialize()
{

	//MFnSets
	MFnEnumAttribute eAttr;
	MFnNumericAttribute nAttr;


	//a_verbose
	a_verbose = eAttr.create("verbose", "verbose", 0);
	eAttr.addField("no verbose", 0);
	eAttr.addField("verbose", 1);
	addAttribute(a_verbose);

	//a_input_color
	a_input_color = nAttr.createColor("input_color", "input_color");
	nAttr.setUsedAsColor(true);
	nAttr.setStorable(false);
	addAttribute(a_input_color);

	//a_output_color
	a_output_color = nAttr.createColor("output_color", "output_color");
	nAttr.setUsedAsColor(true);
	nAttr.setStorable(false);
	addAttribute(a_output_color);



	//Attribute affects
	attributeAffects(a_input_color, a_output_color);

	return MStatus::kSuccess;
}

//postConstructor
//-----------------------------------------------
void Ocio_simple_transform::postConstructor()
{
	//node
	MObject node = thisMObject();

	//create config
	config = OCIO_functionality::get_config_from_env();
	
	//processor
	std::string input_colorspace("sRGB");
	std::string output_colorspace("SLog");
	processor = OCIO_functionality::get_processor(config, input_colorspace, output_colorspace);
	

	//temp
	MGlobal::displayInfo("Postconstructor executed");
};

//compute
//-----------------------------------------------
MStatus Ocio_simple_transform::compute(const MPlug &plug, MDataBlock &data)
{
	//wrong plug requested
	//-----------------------------------------------
	if ((plug != a_output_color) && (plug.parent() != a_output_color))
	{
		return MStatus::kUnknownParameter;
	}

	//right plug requested
	//-----------------------------------------------

	//get attributes from datablock
	//-----------------------------------------------
	int verbose = static_cast<int>(data.inputValue(a_verbose).asShort());
	//vec_input_color
	MDataHandle h_input_color = data.inputValue(a_input_color);
	MFloatVector& vec_input_color = h_input_color.asFloatVector();



	//computation
	//-----------------------------------------------

	//print_config_info
	//print_config_info();

	//pixels
	float pixels[3];
	vec_input_color.get(pixels);

	//planar_image
	OCIO::PlanarImageDesc planar_image(&pixels[0], &pixels[1], &pixels[2], NULL, 1, 1);
	//apply transform
	processor->apply(planar_image);

	//set colors back to vec_input_color
	vec_input_color.x = pixels[0];
	vec_input_color.y = pixels[1];
	vec_input_color.z = pixels[2];

	//set output color
	set_output_color(vec_input_color, data);

	
	
	//log
	//if (verbose)
	//	MGlobal::displayInfo("Compute executed");
	//return success
	return MStatus::kSuccess;
	
}










//Own
//-----------------------------------------------


//attr_connected
bool Ocio_simple_transform::attr_connected(std::string attr_name)
{
	//node
	MObject node = thisMObject();
	//fn_dep_node
	MFnDependencyNode fn_dep_node(node);
	//p_plug
	MPlug p_plug = fn_dep_node.findPlug(attr_name.c_str(), true);

	//return
	return p_plug.isConnected();
};

//set_output_color
void Ocio_simple_transform::set_output_color(MFloatVector& vec_color, MDataBlock& data)
{
	//h_output_color
	MDataHandle h_output_color = data.outputValue(a_output_color);
	MFloatVector& vec_output_color = h_output_color.asFloatVector();
	vec_output_color = vec_color;

	//clean handle
	h_output_color.setClean();

	//tmp
	//MGlobal::displayInfo("Set output color attribute");
};








//Temp
//-----------------------------------------------
//print_config_info
void Ocio_simple_transform::print_config_info()
{
	OCIO_functionality::print_config_info(config);
};