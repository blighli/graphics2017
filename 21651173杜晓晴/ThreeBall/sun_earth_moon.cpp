//
//  sun_earth_moon.cpp
//  ThreeBall
//
//  All rights reserved.
//
#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLGeometryTransform.h>  
#include <GLBatch.h>
#include <StopWatch.h>

#include <math.h>
#include <glut/glut.h>

GLShaderManager shaderManager;
GLTriangleBatch sphereBatch1;
GLTriangleBatch sphereBatch2;
GLTriangleBatch sphereBatch3;
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
GLGeometryTransform transfromPipeline;
GLFrustum vieFrustum;
GLFrame camerFrame;

void SetupRC(){
    shaderManager.InitializeStockShaders();
    
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    gltMakeSphere(sphereBatch1, 0.5f, 100, 50);
    
    gltMakeSphere(sphereBatch2, 0.1f, 26, 13);
    
    gltMakeSphere(sphereBatch3, 0.02f, 12, 6);
    
    
    
}




//llaa


void ChangeSize(int w,int h){
    glViewport(0, 0, w, h);
    
    vieFrustum.SetPerspective(35.0f, float(w)/float(h), 1.0f, 100.0f);
    projectionMatrix.LoadMatrix(vieFrustum.GetProjectionMatrix());
    
    transfromPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
    
    
}

void RenderScene(void){
    static GLfloat vGreenColor[]={0.0f,1.0f,0.0f,1.0f};
    static GLfloat vRedColor[]={1.0f,0.0f,0.0f,1.0f};
    static GLfloat  vBlueColor[]={0.0f,0.0f,1.0f,1.0f};
    
    static CStopWatch rotTimer;
    float yRot=rotTimer.GetElapsedSeconds()*60.0f;
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    modelViewMatrix.PushMatrix();
    
    M3DMatrix44f mCamera;
    camerFrame.GetCameraMatrix(mCamera);
    modelViewMatrix.PushMatrix(mCamera);
    
    M3DVector4f vLightPos={0.0f,6.0f,5.0f,1.0f};
    M3DVector4f vLightEyePos;
    m3dTransformVector4(vLightEyePos, vLightPos, mCamera);
    
    modelViewMatrix.Translate(0.0f, 0.0f, -2.5f);
    
    
    shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,transfromPipeline.GetModelViewMatrix(),transfromPipeline.GetProjectionMatrix(),vLightEyePos,vRedColor);
    sphereBatch1.Draw();
    
    
    
    modelViewMatrix.Rotate(yRot * -2.0f, 0.0f, 1.0f, 0.0f);
    modelViewMatrix.Translate(0.8f, 0.0f, 0.0f);
    shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,transfromPipeline.GetModelViewMatrix(),transfromPipeline.GetProjectionMatrix(),vLightEyePos,
                                 vBlueColor);
    
    sphereBatch2.Draw();
    
    
    
    
    modelViewMatrix.Rotate(yRot*3,1.0f,0.0f,0.0f);
    
    modelViewMatrix.Translate(0.1f,0.1f,0.0f);
    
    
    shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,transfromPipeline.GetModelViewMatrix(),transfromPipeline.GetProjectionMatrix(),vLightEyePos,
                                 vGreenColor);
    
    sphereBatch3.Draw();
    
    modelViewMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();
    glutSwapBuffers();
    
    glutPostRedisplay();
}

int main(int argc, char* argv[]){
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    
    glutCreateWindow("sun_earth");
    
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    
    
    SetupRC();
    glutMainLoop();
    return 0;
}










