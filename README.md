# COMP3015-Coursework1

## Environment

- **Visual Studio Version:** [17.7.4]
- **Operating System:** [Windows 11 Home Chinese Version]

## How Does It Work?

### Initialization (`SceneBasic_Uniform::initScene()`):

- The scene initialization function sets up shaders, loads textures, creates vertex buffer objects (VBOs) for vertices, normals, and texture coordinates, and configures vertex array objects (VAOs) for rendering.
- It loads various textures from image files and applies them to different geometric shapes.
- The function also sets up perspective projection and lighting information.

### Rendering (`SceneBasic_Uniform::render()`):

- This function clears the screen and depth buffer, prepares the transformation matrices (model, view, projection), and updates uniform variables in the shaders.
- It then binds textures, sets up model transformations for each object (translation, rotation, scaling), and draws the objects.
- Finally, it renders a model object (a banana in this case) with a specific transformation and texture.

### Shader Compilation (`SceneBasic_Uniform::compile()`):

- Compiles vertex and fragment shaders, links them into a shader program, and checks for errors.
- The shaders are used to apply lighting and texture mapping to the rendered objects.

### Texture Loading (`loadTexture()`):

- This utility function reads an image file and creates an OpenGL texture, setting appropriate parameters for wrapping and filtering.

## Navigating the Code

### Understanding Class Structure:

- The code likely belongs to a `SceneBasic_Uniform` class that encapsulates all the logic for setting up and rendering a particular scene.

### Following Method Calls:

- Trace how OpenGL's state is set up in `initScene` and how it's used in `render`. The setup involves creating buffers, textures, and shader programs, while rendering involves binding these resources and drawing.

### Shader Files:

- The actual lighting and texturing effects are defined in GLSL shaders, not shown in the snippet. To fully understand the rendering, one would need to look at the associated vertex (`basic_uniform.vert`) and fragment (`basic_uniform.frag`) shader code.

### Modifying Objects:

- To change the scene, a programmer could adjust transformation matrices, swap textures, or modify the shader code.

## Additional Insights

### Extensibility:

- The structure suggests that adding new objects or effects involves creating new buffer objects and shaders or modifying existing ones.

### Debugging:

- The use of `std::cerr` for error reporting in the shader compilation and linking stages helps identify issues at runtime.

### Performance Considerations:

- The code uses VAOs and VBOs for efficient rendering and separates the logic into initialization and per-frame rendering steps to optimize performance.
