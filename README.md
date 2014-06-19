
OpenColorIO_Tools
=================

My set of OpenColorIO tools and plugins. It currently contains plugins for Houdini and Maya, as well as a tiny Qt standalone application.

Abstract - What is OCIO
-----------------------

<p style="float: left;">
	<a href="http://www.youtube.com/watch?v=MjsDBI0UOYU"><img src="http://www.kiiia.com/opencolorio/opencolorio.jpg"></a>
	<a href="http://www.youtube.com/watch?v=fxIoVr9fBVU"><img src="http://www.kiiia.com/opencolorio/opencolorio_nuke.jpg"></a>
	<a href="http://www.youtube.com/watch?v=fE5MOpmcQE0"><img src="http://www.kiiia.com/opencolorio/opencolorio_mari.jpg"></a>
	<a href="http://timmwagener.com/ocio.html"><img src="http://www.kiiia.com/opencolorio/opencolorio_houdini.jpg"></a>
</p>

-----------------------

[OpenColorIO](http://opencolorio.org/) is a library developed by Sony Pictures Imageworks, where it is part of their OpenSource initiative amongst other libraries and frameworks like OIIO, Alembic and OSL to name few.
[OCIO](http://opencolorio.org/) aims to enable consistent color management across programs by providing a unified color environment. Central to this is the idea of .ocio files which define the working color environment, by specifying
 a set of LUTs that can be used. [OCIO](http://opencolorio.org/) provides an interface to a variety of LUT formats, in much the same way as OIIO does to image formats. These LUTs are then used for colortransformation on the CPU or GPU.
[OCIO](http://opencolorio.org/) is increasingly used in commercial software like NUKE or MARI. [here](http://opencolorio.org/CompatibleSoftware.html) is a list of compatible software.

-----------------------

Goal of OpenColorIO_Tools
-------------------------

Since [OCIO](http://opencolorio.org/) is a color management solution geared towards unifying the color management in an animation/vfx pipeline it makes sense for all tools in the pipeline to natively support it.
Therefore the primary goal is to create a set of Houdini COP nodes (Houdinis compositing  network) as equivalents to the NUKE OCIO nodes which ship natively.
Additionaly i plan on developing a set of Maya nodes for color managing textures, to become familiar with the library.
My personal interest in this project is to enhance my C++ programming and to become familiar with the Houdini Development Kit.

-----------------------

Nuke OCIO nodes for Houdini
---------------------------

The list of nodes i want to make available as Houdini COP2 nodes

* **OCIOCDLTransform**
	Applies an ASC CDL (American Society of Cinematographers Color Decision List) grade based on the OpenColorIO Library
* **OCIOColorSpace**
	Apply a color transformation based on a .ocio file. Ocio files are yaml based and describe the complete available colorenvironment. The color environment in turn usualy consists of a dozen Lut files which describe transformations to/from the reference color space to the target space.
* **OCIOFileTransform**
	Accepts a path to a specific color lut file and does a color transformation based on it.
* **OCIOLogConvert**
	Do a color transformation from Lin to Log, or the other way round, based on the Lin and Log roles of the given .ocio config file.


For more infos visit [this](http://www.timmwagener.com/ocio.html).