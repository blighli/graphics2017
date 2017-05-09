功能：加载模型并显示，有前后两个光源，实现了两种视点变换。
平台：VS2012，ASSIMP库，最后引入的lib文件有：assimp.lib，glew32s.lib，glfw3.lib，OpenGL32.lib，SOIL.lib。

在网上找的一份DEMO上进行的修改。源码在ModelImport文件夹中，模型在nanosuit文件夹中。

A、D键在照相机左右方向上平移，W、S键在照相机前后方向上平移。
鼠标控制照相机的视线方向，滚轮改变投影矩阵。但是滚轮缩放有问题，一直向前滚回看到模型的上下颠倒，不知道为什么。
ESC键关闭程序。
上下左右键让照相机围绕模型旋转，但因为没有参照物，所以看起来的效果只是模型在旋转。
SPACE键控制背面红光的开启和关闭。

程序用到的类有：Camera，Mesh，Model，Shader，TextureHelper。另外程序用到了顶点着色器和片段着色器。

本人添加的代码在50行左右，集中：
loadModeWithLightl.cpp的146-162行（添加背面红光）；
loadModeWithLightl.cpp的241-250行（相机围绕模型旋转和背面光的开/关）
camera.h的68-79行（设置相机的旋转矩阵）
model.frag的52-62行，以及修改70行（两个光源的混合结果）

对不起，没有手动去写模型的载入……
另外，这个DEMO的代码风格真的好棒。