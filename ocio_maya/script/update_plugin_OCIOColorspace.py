

#update_plugin
#-----------------------------------------
'''
1.Unload plugin and force new file opening in Maya
2.Copy plugin from source to target path
3.Reload plugin
4.Create setup
'''

#Import
#-----------------------------------------
#python
import os
import sys
import shutil
#maya
import maya.cmds as cmds
import pymel.core as pm



#Vars
#-----------------------------------------
verbose = True
plugin_name = 'ocio_maya.mll'
source_dir = 'C:/symlinks/paper_sketch/OpenColorIO_tools/ocio_maya_build/x64/msvc10/maya2014x64/Release'
target_dir = 'C:/symlinks/maya/maya2014x64_plugins'
texture_name = 'enzo_v0001_tw.png' #base_color_test_image.png
texture_dir = 'C:/symlinks/temp'


#Unload plugin
#-----------------------------------------
#open new file
cmds.file(new = True, f = True)
#unload
pm.unloadPlugin(plugin_name)
if(verbose): 
		print('Unloaded plugin: {0}'.format(plugin_name))


#Copy plugin
#-----------------------------------------
try:
	shutil.copy2(source_dir +'/' +plugin_name, target_dir)
	if(verbose): print('Copied plugin from {0} to {1}'.format(source_dir +'/' +plugin_name,
																		target_dir))
except:
	if(verbose): print('Error copying plugin from {0} to {1}'.format(source_dir +'/' +plugin_name,
																		target_dir))

#Reload plugins
#-----------------------------------------
try:
	pm.loadPlugin(plugin_name)
	if(verbose): 
		print('Reloaded plugin: {0}'.format(plugin_name))
except:
	if(verbose): 
		print('Error loading plugin: {0}'.format(plugin_name))


#Test setup
#-----------------------------------------
#-----------------------------------------

#ocio_node
ocio_node = pm.createNode('OCIOColorspace')
pm.rename(ocio_node, 'OCIOColorspace_1')
ocio_node.verbose.set(1)
pm.select(cl = True)

#file_node
file_node = pm.shadingNode('file', name='file_tex', ss = True, asTexture = True)
#place2d_node
place2d_node = pm.createNode('place2dTexture')
pm.select(cl = True)

#shader_node
shader_node = pm.shadingNode('surfaceShader',name='surface_mt',asShader = True)
#shading_group_node
shading_group_node = pm.sets(renderable = True, empty = True, name= shader_node.name() +'_SG')

#connect file and place2d
place2d_node.coverage >> file_node.coverage
place2d_node.mirrorU >> file_node.mirrorU
place2d_node.mirrorV >> file_node.mirrorV
place2d_node.noiseUV >> file_node.noiseUV
place2d_node.offset >> file_node.offset
place2d_node.outUV >> file_node.uvCoord
place2d_node.outUvFilterSize >> file_node.uvFilterSize
place2d_node.repeatUV >> file_node.repeatUV
place2d_node.rotateFrame >> file_node.rotateFrame
place2d_node.rotateUV >> file_node.rotateUV
place2d_node.stagger >> file_node.stagger
place2d_node.translateFrame >> file_node.translateFrame
place2d_node.vertexCameraOne >> file_node.vertexCameraOne
place2d_node.vertexUvOne >> file_node.vertexUvOne
place2d_node.vertexUvThree >> file_node.vertexUvThree
place2d_node.vertexUvTwo >> file_node.vertexUvTwo
place2d_node.wrapU >> file_node.wrapU
place2d_node.wrapV >> file_node.wrapV
pm.select(cl = True)

#connect shader to shading group
pm.connectAttr( shader_node.name() + ".outColor", shading_group_node.name() + ".surfaceShader", force = True)
pm.select(cl = True)

#connect file to ocio node
file_node.outColor >> ocio_node.input_color
pm.select(cl = True)

#connect ocio node to shader
ocio_node.output_color >> shader_node.outColor
pm.select(cl = True)

#set texture file
file_node.fileTextureName.set(texture_dir +'/' +texture_name)

#polyplane_transform_node, polyplane_shape_node
polyplane_transform_node, polyplane_shape_node = pm.polyPlane(n = 'ocio_test_polyplane', sx = 10, sy = 10, axis = (0,1,0), width = 12.8, height = 7.8)
pm.select(cl = True)
#connect plane to shader
pm.sets(shading_group_node, forceElement = polyplane_transform_node.name())
pm.select(cl = True)

#render_cam
render_cam_transform, render_cam_shape = pm.camera()
pm.select(cl = True)
pm.rename(render_cam_transform, 'render_cam')
render_cam_transform.translate.set(0,13,7)
render_cam_transform.rotate.set(-65,0,0)
pm.setKeyframe(render_cam_transform, s = False)
pm.lookThru(render_cam_transform)


#select ocio node at the end
pm.select(ocio_node, r = True)