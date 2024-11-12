# TODO
- Add a way to render Layer in part of the screen using different viewport:
see https://www.reddit.com/r/opengl/comments/2s4f6s/render_to_only_part_of_screen/?rdt=44764
see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glViewportIndexed.xhtml
This might need to bump up to opengl 4.1
- Uniformize opengl versionning. cmake var directly copying to shaders?
- Add a "Entity" that have child entitys that can be models, or light for example.
- drag and drop using https://www.glfw.org/docs/3.3/input_guide.html#path_drop
- consider replacing the m_visible in the onUpdate of layers by a mecanism to remove layer from the window vector
- Add onAttach and onDetach to Layer (and init values in here in apps)
- add docking in the whole window
- fix camera movement on top and bottom edge
- add fbo abstractions
- Optimize copys between scene and layers/renderer
- Change PerspectiveCamera of FpsCameraMover by general Camera or Entity
- Move differents ui panels to different files/classes
