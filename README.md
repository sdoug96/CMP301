# CMP301-Coursework

Assessment overview

This coursework comprises two parts: the graphics application and a report detailing the work
completed.

Task 1: Graphics Application (50%)

Develop your own shaders and an accompanying application to demonstrate them. Create a
single scene to demonstrate the shaders created. This scene does not have to be overly large
or populated; the focus is on the use of the programmable pipeline. The brief is designed to
be as open as possible providing you the opportunity to develop something of your own
interests and design. Ideally the coursework should demonstrate the use of all the shader
stages: vertex, pixel, tessellation (including Hull and Domain), and geometry. The scene
should demonstrate key graphical techniques:

? Must contain an example of vertex manipulation

o The geometry should be correctly textured and lit after manipulation.

? Must contain a post processing technique

o Post processing must make use of render to texture.

o Demonstrate pixel manipulation on a pixel by pixel basis.

? Lighting and shadows

o All geometry must have correctly calculated/transformed normals and must
make use of correct lighting algorithms.

For an improved grade your scene should also demonstrate:

? Tessellation

o The tessellation should be non-trivial and impact the scene ie do not just
tessellate a flat surface.

o Tessellated geometry should be correctly textured and lit.

o Consider dynamically controlled tessellation.

? Geometry Shader Stage

o Non-trivial geometry manipulation and/or generation

o Geometry created or processed by this stage should be correctly textured
and lit.

Your project should utilise an object oriented approach with appropriated classes. All code
should be well structured and well commented. You should differentiate your coursework from
the examples provided in lectures and labs. Do not return my own code examples to me.
The application must run on the lab computers. The scene must demonstrate your ability to
synthesise concepts from class into a single scene that demonstrates lighting, texturing,
manipulation, post process together, not separately.

The minimum requirements for this coursework are a working and non-trivial set of vertex and
pixel shaders that demonstrate simple vertex and pixel manipulation (e.g. texture blending
and lighting, or post processing). You should aim to demonstrate all shader stages.

Task 2: Report (50%)

Your application must be accompanied by a report that will highlight how you addressed the
coursework requirements. This report should include the following:

1. Overview of the scene

o What objects are in the scene and what techniques they demonstrate
o How you have responded to the coursework brief
o Outline any UI elements and controls beyond the standard controls

2. Algorithms and Data Structures

o An in-depth explanation and justification (based on complexity and/or
hardware architecture) of the algorithms and data structures used in the
scene
o Important calculations used, data passed and shader stages
o This should focus on the hlsl/shaders written
o Providing diagrams, code snippets and supporting screenshots as required

3. Critical reflection of the application

o Discussion of what your learned, any shortfalls, areas for improvements, how
you might extend the application, what you would do differently if doing it over
again. Offer possible solutions to the challenges or how it could be taken
forward.
o This should refer to the coursework plan done during week 7/8

4. References

o References for any images, models or techniques incorporated in your
application. Cases of plagiarism will be taken very seriously.
A sufficiently detailed document will be between 4-8 pages. Best practice is to write this
document as if explaining to another student how your code works and is constructed.

Submission

Electronically via Blackboard by Tuesday 11th December 2018.
? The submission should include the source code/project, an executable file and
documentation. These files should be contained within a zip file for uploading, using
the following folder structure:

o A folder titled "source" containing the entire Visual Studio project. You can
exclude the project SQL file and the “ipch” folder as these will increase the
size of your submission but are not required.

o A folder titled "exe" containing the executable file and any shaders or other
assets required for the project to run standalone.

o A folder titled “docs” containing a PDF of your report.
All submissions must be uploaded to the appropriate location within the Blackboard system.
You will be able to have multiple submissions (in case of errors) but only the last submission
will be marked.
