### OpenGLThirdWork

#### 1.Obj支持

obj文件通过Main.java的args参数进行传入

#### 2.多个光源光照	
        GL11.glEnable(GL11.GL_LIGHTING);
        GL11.glEnable(GL11.GL_LIGHT0);
        GL11.glEnable(GL11.GL_LIGHT1);
    
        float lightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        float lightDiffuse[] = { 0.5f, 0.5f, 1.0f, 1.0f };
        float lightSpecular[] = { 0.0f, 1.0f, 1.0f, 0.0f };
    
        GL11.glLight(GL11.GL_LIGHT0, GL11.GL_AMBIENT, asFloatBuffer(lightAmbient));
        GL11.glLight(GL11.GL_LIGHT0, GL11.GL_DIFFUSE, asFloatBuffer(lightDiffuse));
        GL11.glLight(GL11.GL_LIGHT0, GL11.GL_SPECULAR, asFloatBuffer(lightSpecular));
    
        GL11.glLight(GL11.GL_LIGHT1, GL11.GL_AMBIENT, asFloatBuffer(lightAmbient));
        GL11.glLight(GL11.GL_LIGHT1, GL11.GL_DIFFUSE, asFloatBuffer(lightDiffuse));
        GL11.glLight(GL11.GL_LIGHT1, GL11.GL_SPECULAR, asFloatBuffer(lightSpecular));
    
        float lightPosition0[] = { posix, posiy, posiz, 1.0f };
        GL11.glLight(GL11.GL_LIGHT0, GL11.GL_POSITION, asFloatBuffer(lightPosition0));
        float lightPosition1[] = { posix, posiy, posizz, 1.0f };
        GL11.glLight(GL11.GL_LIGHT1, GL11.GL_POSITION, asFloatBuffer(lightPosition1));

#### 3.视点浏览

##### a.以模型为中心的旋转

分别使用`W` `S` `A` `D`键完成

##### b.以视点为中心的旋转

分别使用`上` `下` `左` `右`方向键完成